// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_OPERATION_LIST_INPUT_H
#define SCHED_SHOP_OPERATION_LIST_INPUT_H

#include <algorithm>
#include <string>
#include <vector>

#include <sched/Ids.h>
#include <sched/meta/Instance.h>
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

      for (const JobId job : jobs(instance)) {
        for (const OperationId operation : operations(instance, job)) {
          input.push_back(operation);
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
      std::vector<JobId> job_list;

      for (const JobId job : jobs(instance)) {
        const std::size_t operation_count = instance.operation_count(job);
        job_list.insert(job_list.end(), operation_count, job);
      }

      std::shuffle(job_list.begin(), job_list.end(), random);

      std::vector<std::size_t> operations(instance.job_count(), 0);

      shop::OperationListInput input;

      for (const JobId job : job_list) {
        const std::size_t index = operations[to_index(job)]++;
        const OperationId operation = { .job = job, .index = index };
        input.push_back(operation);
      }

      return input;
    }
  };

}

#endif // SCHED_SHOP_OPERATION_LIST_INPUT_H
