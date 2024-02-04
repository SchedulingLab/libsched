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
    Input operator()(const Input& input, Random& random)
    {
      assert(!input.empty());

      const std::size_t max = input.size() - 1;
      Input neighbor = input;

      while (neighbor == input) {
        std::size_t orig = 0;
        std::size_t dest = 0;

        while (orig == dest) {
          orig = random.compute_uniform_integer(std::size_t(0), max);
          dest = random.compute_uniform_integer(std::size_t(0), max);
        }

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
      }

      return neighbor;
    }
  };

  template<>
  struct MutationTraits<InsertMutation> {
    static std::string name()
    {
      return "ins";
    }
  };

}

#endif // SCHED_INSERT_MUTATION_H
