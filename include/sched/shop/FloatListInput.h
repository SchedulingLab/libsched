#ifndef SCHED_SHOP_RANDOM_LIST_INPUT_H
#define SCHED_SHOP_RANDOM_LIST_INPUT_H

#include <string>
#include <vector>

#include <sched/common/Instance.h>
#include <sched/common/Random.h>
#include <sched/common/Range.h>

#include "InputTraits.h"

namespace sched::shop {

  struct FloatListInput : std::vector<double> {
    using std::vector<double>::vector;
  };

  template<>
  struct InputTraits<FloatListInput> {
    static std::string name() {
      return "flt";
    }

    template<typename Instance>
    static FloatListInput generate_input(const Instance& instance) {
      std::size_t operation_count = 0;

      for (auto job : sched::jobs(instance)) {
        operation_count += instance.operation_count(job);
      }

      FloatListInput input(operation_count, 0.0);
      return input;
    }

    template<typename Instance>
    static FloatListInput generate_random(const Instance& instance, Random& random) {
      std::size_t operation_count = 0;

      for (auto job : sched::jobs(instance)) {
        operation_count += instance.operation_count(job);
      }

      FloatListInput input(operation_count);
      std::uniform_real_distribution<double> dist(0.0, 1.0);

      for (auto & item : input) {
        item = dist(random);
      }

      return input;
    }

    template<typename Instance>
    static FloatListInput generate_feasible(const Instance& instance, Random& random) {
      return generate_random(instance, random);
    }

  };
}

#endif // SCHED_SHOP_RANDOM_LIST_INPUT_H
