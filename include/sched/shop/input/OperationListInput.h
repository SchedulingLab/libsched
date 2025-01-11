// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_OPERATION_LIST_INPUT_H
#define SCHED_SHOP_OPERATION_LIST_INPUT_H

#include <algorithm>
#include <string>
#include <vector>

#include <sched/Ids.h>
#include <sched/Instance.h>
#include <sched/support/Random.h>
#include <sched/types/InputTraits.h>

namespace sched {

  namespace shop {

    using OperationListInput = std::vector<OperationId>;

  }

  template<>
  struct InputTraits<shop::OperationListInput> {
    static std::string name()
    {
      return "ope";
    }

    template<typename Instance>
    static shop::OperationListInput generate_input(const Instance& instance)
    {
      shop::OperationListInput input;

      for (auto job : sched::jobs(instance)) {
        auto operations = instance.operation_count(job);
        for (std::size_t i = 0; i < operations; ++i) {
          input.push_back({ job, i });
        }
      }

      return input;
    }

    template<typename Instance>
    static shop::OperationListInput generate_random(const Instance& instance, Random& random)
    {
      shop::OperationListInput input = generate_input(instance);
      std::shuffle(input.begin(), input.end(), random);
      return input;
    }

    template<typename Instance>
    static shop::OperationListInput generate_feasible(const Instance& instance, Random& random)
    {
      // generate a job list
      std::vector<JobId> ids;

      for (auto job : sched::jobs(instance)) {
        auto operations = instance.operation_count(job);

        for (std::size_t i = 0; i < operations; ++i) {
          ids.push_back(job);
        }
      }

      // make the job list random
      std::shuffle(ids.begin(), ids.end(), random);

      // create operations from job ids
      std::vector<std::size_t> jobs(instance.job_count(), 0);

      shop::OperationListInput input;

      for (auto job : ids) {
        OperationId op = {};
        op.job = job;
        op.index = jobs[sched::to_index(job)]++;
        input.push_back(op);
      }

      return input;
    }
  };

}

#endif // SCHED_SHOP_OPERATION_LIST_INPUT_H
