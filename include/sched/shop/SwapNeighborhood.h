#ifndef SCHED_SWAP_NEIHGBORHOOD_H
#define SCHED_SWAP_NEIHGBORHOOD_H

#include <cassert>

#include <sched/common/Api.h>
#include <sched/common/Random.h>
#include <sched/common/Range.h>

namespace sched::shop {

  struct SCHED_API SwapNeighborhood {

    template<typename Input, typename Schedule>
    Input operator()(const Input& input, [[maybe_unused]] const Schedule& schedule, Random& random) {
      assert(!input.empty());

      std::size_t max = input.size() - 1;
      std::size_t n1, n2;

      do {
        n1 = random.compute_uniform_integer(std::size_t{0}, max);
        n2 = random.compute_uniform_integer(std::size_t{0}, max);
      } while (n1 == n2 || input[n1] == input[n2]);

      Input neighbor = input;
      std::swap(neighbor[n1], neighbor[n2]);
      return neighbor;
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


}

#endif // SCHED_SWAP_NEIHGBORHOOD_H
