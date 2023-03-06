#ifndef SCHED_REVERSE_MUTATION_H
#define SCHED_REVERSE_MUTATION_H

#include <cassert>
#include <algorithm>
#include <string>

#include <sched/common/Api.h>
#include <sched/common/Random.h>

#include "MutationTraits.h"

namespace sched::shop {

  struct SCHED_API ReverseMutation {

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
        std::reverse(&neighbor[endpoints[0]], &neighbor[endpoints[1]] + 1);
      } while (neighbor == input);

      return neighbor;
    }

  };

  template<>
  struct MutationTraits<ReverseMutation> {
    static std::string name() {
      return "rev";
    }
  };

}

#endif // SCHED_REVERSE_MUTATION_H
