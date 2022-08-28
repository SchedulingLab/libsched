#ifndef SCHED_SHOP_ADJACENT_SWAP_NEIGHBORHOOD_H
#define SCHED_SHOP_ADJACENT_SWAP_NEIGHBORHOOD_H

#include <cassert>

#include <sched/common/Api.h>
#include <sched/common/Random.h>
#include <sched/common/Range.h>

#include "NeighborhoodHelper.h"

namespace sched::shop {

  struct SCHED_API AdjacentSwapNeighborhood {

    template<typename Input, typename Schedule>
    Input operator()(const Input& input, [[maybe_unused]] const Schedule& schedule, Random& random) {
      assert(!input.empty());

      std::size_t max = input.size() - 1;
      std::size_t index;

      do {
        index = random.compute_uniform_integer(std::size_t{0}, max - 1);
        assert(index < input.size() - 1);
      } while (input[index] == input[index + 1]);

      Input neighbor = input;
      std::swap(neighbor[index], neighbor[index + 1]);
      return neighbor;
    }

    template<typename Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count) {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

    template<typename Input>
    bool are_neighbors(const Input& input0, const Input& input1) {
      assert(input0.size() == input1.size());
      int diff = 0;

      for (auto i : sched::over(input0)) {
        if (input0[i] != input1[i]) {
          ++diff;
        }
      }

      // TODO: also check that the indices are at distance 1
      return diff == 2;
    }

  };

}

#endif // SCHED_SHOP_ADJACENT_SWAP_NEIGHBORHOOD_H
