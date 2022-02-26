#ifndef SCHED_REVERSE_NEIHGBORHOOD_H
#define SCHED_REVERSE_NEIHGBORHOOD_H

#include <cassert>
#include <algorithm>

#include <sched/common/Api.h>
#include <sched/common/Random.h>
#include <sched/common/Range.h>

namespace sched::shop {

  struct SCHED_API ReverseNeighborhood {
    template<typename Input>
    Input operator()(const Input& input, Random& random) {
      assert(!input.empty());

      std::size_t max = input.size() - 1;
      Input neighbor;

      do {
        std::size_t endpoints[2];

        do {
          endpoints[0] = random.compute_uniform_integer(std::size_t{0}, max);
          endpoints[1] = random.compute_uniform_integer(std::size_t{0}, max);
        } while (endpoints[0] == endpoints[1]);

        if (endpoints[0] > endpoints[1]) {
          std::swap(endpoints[0], endpoints[1]);
        }

        neighbor = input;
        std::reverse(neighbor.begin() + endpoints[0], neighbor.begin() + endpoints[1] + 1);
      } while (neighbor == input);

      return neighbor;
    }

    template<typename Input>
    bool are_neighbors(const Input& input0, const Input& input1) {
      std::size_t size = input0.size();
      assert(size > 0);
      assert(input1.size() == size);

      std::size_t orig = 0;

      for (; orig < size; ++orig) {
        if (input0[orig] != input1[orig]) {
          break;
        }
      }

      std::size_t dest = size - 1;

      for (; dest > 0; --dest) {
        if (input0[dest] != input1[dest]) {
          break;
        }
      }

      if (orig >= dest) {
        return false;
      }

      for (std::size_t i = 0; i <= dest - orig; ++i) {
        if (input0[orig + i] != input1[dest - i]) {
          return false;
        }
      }

      return true;
    }
  };


}

#endif // SCHED_REVERSE_NEIHGBORHOOD_H
