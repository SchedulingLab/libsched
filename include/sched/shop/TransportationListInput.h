#ifndef SCHED_SHOP_TRANSPORTATION_LIST_INPUT_H
#define SCHED_SHOP_TRANSPORTATION_LIST_INPUT_H

#include <algorithm>
#include <vector>

#include <sched/common/Ids.h>
#include <sched/common/Instance.h>
#include <sched/common/Random.h>

#include "InputTraits.h"
#include "InputSize.h"

namespace sched::shop {

  using TransportationListInput = std::vector<TransportationId>;

  template<>
  struct InputTraits<TransportationListInput> {
    static std::string name() {
      return "trp";
    }

    static void enumerate_start(TransportationListInput& input) {
      std::sort(input.begin(), input.end());
    }

    static bool enumerate_next(TransportationListInput& input) {
      return std::next_permutation(input.begin(), input.end());
    }

    template<typename Instance>
    static TransportationListInput generate_input(const Instance& instance) {
      const std::size_t input_size = input_extended_size_for(instance);
      const std::size_t transportation_count = instance.transportation_count();

      TransportationListInput input;
      input.reserve(input_size);

      for (std::size_t i = 0; i < input_size; ++i) {
        input.push_back(TransportationId{i % transportation_count});
      }

      return input;
    }

    template<typename Instance>
    static TransportationListInput generate_random(const Instance& instance, Random& random) {
      TransportationListInput input = generate_input(instance);
      std::shuffle(input.begin(), input.end(), random);
      return input;
    }

    template<typename Instance>
    static TransportationListInput generate_feasible(const Instance& instance, Random& random) {
      // all inputs are feasible
      return generate_random(instance, random);
    }

  };

}

#endif // SCHED_SHOP_TRANSPORTATION_LIST_INPUT_H
