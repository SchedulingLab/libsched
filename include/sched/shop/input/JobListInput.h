// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_LIST_INPUT_H
#define SCHED_SHOP_JOB_LIST_INPUT_H

#include <algorithm>
#include <string>
#include <vector>

#include <sched/Ids.h>
#include <sched/meta/Instance.h>
#include <sched/support/Random.h>
#include <sched/types/InputTraits.h>

namespace sched {

  namespace shop {

    using JobListInput = std::vector<JobId>;

  }

  template<>
  struct InputTraits<shop::JobListInput> {
    static std::string name()
    {
      return "job";
    }

    template<typename Instance>
    static shop::JobListInput generate_input(const Instance& instance)
    {
      shop::JobListInput input;

      for (auto job : sched::jobs(instance)) {
        auto operations = instance.operation_count(job);

        for (std::size_t i = 0; i < operations; ++i) {
          input.push_back(job);
        }
      }

      return input;
    }

    template<typename Instance>
    static shop::JobListInput generate_random(const Instance& instance, Random& random)
    {
      shop::JobListInput input = generate_input(instance);
      std::shuffle(input.begin(), input.end(), random);
      return input;
    }

    template<typename Instance>
    static shop::JobListInput generate_feasible(const Instance& instance, Random& random)
    {
      // all inputs are feasible
      return generate_random(instance, random);
    }
  };

}

#endif // SCHED_SHOP_JOB_LIST_INPUT_H
