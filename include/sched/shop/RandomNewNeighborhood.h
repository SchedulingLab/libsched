#ifndef SCHED_SHOP_RANDOM_NEW_NEIGHBORHOOD_H
#define SCHED_SHOP_RANDOM_NEW_NEIGHBORHOOD_H

#include <cassert>
#include <random>
#include <string>

#include <sched/common/Api.h>

#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"
#include "FloatListInput.h"

namespace sched::shop {

  struct SCHED_API RandomNewNeighborhood {

    template<typename Schedule>
    FloatListInput operator()(const FloatListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random) {
      assert(!input.empty());
      std::uniform_real_distribution<double> dist_value(0.0, 1.0);
      std::bernoulli_distribution dist_change(0.15);
      FloatListInput neighbor = input;

      while (neighbor == input) {
        for (auto & value : neighbor) {
          if (dist_change(random)) {
            value = dist_value(random);
          }
        }
      }

      return neighbor;
    }

    template<typename Schedule>
    std::vector<FloatListInput> generate_many(const FloatListInput& input, const Schedule& schedule, Random& random, std::size_t count) {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

  };

  template<>
  struct NeighborhoodTraits<RandomNewNeighborhood> {
    static std::string name() {
      return "rnd";
    }
  };

}

#endif // SCHED_SHOP_RANDOM_NEW_NEIGHBORHOOD_H
