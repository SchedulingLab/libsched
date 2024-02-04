#ifndef SCHED_SHOP_OPERATION_LIST_INPUT_H
#define SCHED_SHOP_OPERATION_LIST_INPUT_H

#include <algorithm>
#include <string>
#include <vector>

#include <sched/common/Ids.h>
#include <sched/common/Instance.h>
#include <sched/common/Random.h>

#include "InputTraits.h"

namespace sched::shop {

  using OperationListInput = std::vector<OperationId>;

  template<>
  struct InputTraits<OperationListInput> {
    static std::string name()
    {
      return "ope";
    }

    static void enumerate_start(OperationListInput& input)
    {
      std::sort(input.begin(), input.end());
    }

    static bool enumerate_next(OperationListInput& input)
    {
      return std::next_permutation(input.begin(), input.end());
    }

    template<typename Instance>
    static OperationListInput generate_input(const Instance& instance)
    {
      OperationListInput input;

      for (auto job : sched::jobs(instance)) {
        auto operations = instance.operation_count(job);
        for (std::size_t i = 0; i < operations; ++i) {
          input.push_back({ job, i });
        }
      }

      return input;
    }

    template<typename Instance>
    static OperationListInput generate_random(const Instance& instance, Random& random)
    {
      OperationListInput input = generate_input(instance);
      std::shuffle(input.begin(), input.end(), random);
      return input;
    }

    template<typename Instance>
    static OperationListInput generate_feasible(const Instance& instance, Random& random)
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

      OperationListInput input;

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
