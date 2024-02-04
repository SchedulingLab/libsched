#ifndef SCHED_SHOP_JOB_LIST_INPUT_H
#define SCHED_SHOP_JOB_LIST_INPUT_H

#include <algorithm>
#include <string>
#include <vector>

#include <sched/common/Ids.h>
#include <sched/common/Instance.h>
#include <sched/common/Random.h>

#include "InputTraits.h"

namespace sched::shop {

  using JobListInput = std::vector<JobId>;

  template<>
  struct InputTraits<JobListInput> {
    static std::string name()
    {
      return "job";
    }

    static void enumerate_start(JobListInput& input)
    {
      std::sort(input.begin(), input.end());
    }

    static bool enumerate_next(JobListInput& input)
    {
      return std::next_permutation(input.begin(), input.end());
    }

    template<typename Instance>
    static JobListInput generate_input(const Instance& instance)
    {
      JobListInput input;

      for (auto job : sched::jobs(instance)) {
        auto operations = instance.operation_count(job);

        for (std::size_t i = 0; i < operations; ++i) {
          input.push_back(job);
        }
      }

      return input;
    }

    template<typename Instance>
    static JobListInput generate_random(const Instance& instance, Random& random)
    {
      JobListInput input = generate_input(instance);
      std::shuffle(input.begin(), input.end(), random);
      return input;
    }

    template<typename Instance>
    static JobListInput generate_feasible(const Instance& instance, Random& random)
    {
      // all inputs are feasible
      return generate_random(instance, random);
    }
  };

}

#endif // SCHED_SHOP_JOB_LIST_INPUT_H
