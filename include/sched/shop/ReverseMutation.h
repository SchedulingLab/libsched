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
      Input neighbor = input;

      while (neighbor == input) {
        std::size_t endpoints[2];

        do { // NOLINT(cppcoreguidelines-avoid-do-while)
          endpoints[0] = random.compute_uniform_integer(std::size_t{0}, max);
          endpoints[1] = random.compute_uniform_integer(std::size_t{0}, max);
        } while (endpoints[0] == endpoints[1]);

        if (endpoints[0] > endpoints[1]) {
          std::swap(endpoints[0], endpoints[1]);
        }

        std::reverse(&neighbor[endpoints[0]], &neighbor[endpoints[1]] + 1);
      }

      return neighbor;
    }

  };

  template<>
  struct MutationTraits<ReverseMutation> {
    static std::string name() {
      return "rev";
    }
  };

  struct SCHED_API CappedReverseMutation {

    template<typename Input>
    Input operator()(const Input& input, Random& random) {
      assert(!input.empty());

      std::size_t max = input.size() - 1;
      Input neighbor = input;

      while (neighbor == input) {
        std::size_t endpoints[2];

        do { // NOLINT(cppcoreguidelines-avoid-do-while)
          endpoints[0] = random.compute_uniform_integer(std::size_t{0}, max);
          endpoints[1] = random.compute_uniform_integer(std::size_t{0}, max);

          if (endpoints[0] > endpoints[1]) {
            std::swap(endpoints[0], endpoints[1]);
          }
        } while (endpoints[0] == endpoints[1] || (endpoints[1] - endpoints[0] > std::max(max / 5, std::size_t(5))));

        std::reverse(&neighbor[endpoints[0]], &neighbor[endpoints[1]] + 1);
      }

      return neighbor;
    }

  };

  template<>
  struct MutationTraits<CappedReverseMutation> {
    static std::string name() {
      return "crv";
    }
  };

}

#endif // SCHED_REVERSE_MUTATION_H
