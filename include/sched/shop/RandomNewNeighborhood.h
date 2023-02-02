#ifndef SCHED_SHOP_RANDOM_NEW_NEIGHBORHOOD_H
#define SCHED_SHOP_RANDOM_NEW_NEIGHBORHOOD_H

#include <cassert>
#include <random>
#include <string>

#include <sched/common/Api.h>

#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"
#include "RandomListInput.h"

namespace sched::shop {

  struct SCHED_API RandomNewNeighborhood {

    template<typename Schedule>
    RandomListInput operator()(const RandomListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random) {
      assert(!input.empty());
      RandomListInput neighbor = input;

      std::uniform_int_distribution<std::size_t> dist_index(0, neighbor.size() - 1);
      std::size_t index = dist_index(random);

      std::uniform_real_distribution<double> dist_value(0.0, 1.0);
      neighbor[index] = dist_value(random);

      return neighbor;
    }

    template<typename Schedule>
    std::vector<RandomListInput> generate_many(const RandomListInput& input, const Schedule& schedule, Random& random, std::size_t count) {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

    bool are_neighbors([[maybe_unused]] const RandomListInput& input0, [[maybe_unused]] const RandomListInput& input1) {
      return true;
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
