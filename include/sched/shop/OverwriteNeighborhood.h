#ifndef SCHED_OVERWRITE_NEIHGBORHOOD_H
#define SCHED_OVERWRITE_NEIHGBORHOOD_H

#include <cassert>

#include <sched/common/Api.h>
#include <sched/common/Random.h>
#include <sched/common/Range.h>

#include "NeighborhoodHelper.h"

namespace sched::shop {

  struct SCHED_API OverwriteNeighborhood {

    template<typename Input, typename Schedule>
    Input operator()(const Input& input, [[maybe_unused]] const Schedule& schedule, Random& random) {
      assert(!input.empty());

      std::size_t max = input.size() - 1;
      std::size_t index0, index1;

      do {
        index0 = random.compute_uniform_integer(std::size_t{0}, max);
        index1 = random.compute_uniform_integer(std::size_t{0}, max);
      } while (index0 == index1 || input[index0] == input[index1]);

      Input neighbor = input;
      neighbor[index0] = neighbor[index1];
      return neighbor;
    }

    template<typename Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count) {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

    template<typename Input>
    bool are_neighbors(const Input& input0, const Input& input1) {
      return true;
    }

  };

}

#endif // SCHED_OVERWRITE_NEIHGBORHOOD_H
