#ifndef SCHED_SCRAMBLE_MUTATION_H
#define SCHED_SCRAMBLE_MUTATION_H

#include <cassert>
#include <algorithm>
#include <string>

#include <sched/common/Api.h>
#include <sched/common/Random.h>

#include "MutationTraits.h"

namespace sched::shop {

  struct SCHED_API ScrambleMutation {

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
        std::shuffle(&neighbor[endpoints[0]], &neighbor[endpoints[1]] + 1, random);
      } while (neighbor == input);

      return neighbor;
    }

  };

  template<>
  struct MutationTraits<ScrambleMutation> {
    static std::string name() {
      return "scr";
    }
  };

}

#endif // SCHED_SCRAMBLE_MUTATION_H
