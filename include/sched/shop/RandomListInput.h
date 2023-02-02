#ifndef SCHED_SHOP_RANDOM_LIST_INPUT_H
#define SCHED_SHOP_RANDOM_LIST_INPUT_H

#include <string>
#include <vector>

#include <sched/common/Instance.h>
#include <sched/common/Random.h>
#include <sched/common/Range.h>

#include "InputTraits.h"

namespace sched::shop {

  using RandomListInput = std::vector<double>;

  template<>
  struct InputTraits<RandomListInput> {
    static std::string name() {
      return "rnd";
    }

    template<typename Instance>
    static RandomListInput generate_input(const Instance& instance) {
      std::size_t operation_count = 0;

      for (auto job : sched::jobs(instance)) {
        operation_count += instance.operation_count(job);
      }

      RandomListInput input(operation_count, 0.0);
      return input;
    }

    template<typename Instance>
    static RandomListInput generate_random(const Instance& instance, Random& random) {
      std::size_t operation_count = 0;

      for (auto job : sched::jobs(instance)) {
        operation_count += instance.operation_count(job);
      }

      RandomListInput input(operation_count);
      std::uniform_real_distribution<double> dist(0.0, 1.0);

      for (auto & item : input) {
        item = dist(random);
      }

      return input;
    }

    template<typename Instance>
    static RandomListInput generate_feasible(const Instance& instance, Random& random) {
      return generate_random(instance, random);
    }

  };
}

#endif // SCHED_SHOP_RANDOM_LIST_INPUT_H
