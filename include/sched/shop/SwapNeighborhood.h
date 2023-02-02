#ifndef SCHED_SWAP_NEIHGBORHOOD_H
#define SCHED_SWAP_NEIHGBORHOOD_H

#include <cassert>
#include <string>

#include <sched/common/Api.h>
#include <sched/common/Random.h>
#include <sched/common/Range.h>

#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"

namespace sched::shop {

  struct SCHED_API SwapNeighborhood {

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
      std::swap(neighbor[index0], neighbor[index1]);
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

      return diff == 2;
    }

  };

  template<>
  struct NeighborhoodTraits<SwapNeighborhood> {
    static std::string name() {
      return "swp";
    }
  };

}

#endif // SCHED_SWAP_NEIHGBORHOOD_H
