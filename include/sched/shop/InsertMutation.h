#ifndef SCHED_INSERT_MUTATION_H
#define SCHED_INSERT_MUTATION_H

#include <cassert>
#include <string>

#include <sched/common/Api.h>
#include <sched/common/Random.h>

#include "MutationTraits.h"

namespace sched::shop {

  struct SCHED_API InsertMutation {

    template<typename Input>
    Input operator()(const Input& input, Random& random) {
      assert(!input.empty());

      std::size_t max = input.size() - 1;
      Input neighbor;

      do {
        std::size_t orig, dest;

        do {
          orig = random.compute_uniform_integer(std::size_t{0}, max);
          dest = random.compute_uniform_integer(std::size_t{0}, max);
        } while (orig == dest);

        neighbor = input;
        auto val = neighbor[orig];

        if (orig < dest) {
          for (std::size_t i = orig; i < dest; ++i) {
            neighbor[i] = neighbor[i + 1];
          }
        } else {
          for (std::size_t i = orig; i > dest; --i) {
            neighbor[i] = neighbor[i - 1];
          }
        }

        neighbor[dest] = val;
      } while (neighbor == input);


      return neighbor;
    }

  };

  template<>
  struct MutationTraits<InsertMutation> {
    static std::string name() {
      return "ins";
    }
  };

}

#endif // SCHED_INSERT_MUTATION_H
