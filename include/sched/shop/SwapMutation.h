#ifndef SCHED_SWAP_MUTATION_H
#define SCHED_SWAP_MUTATION_H

#include <cassert>
#include <string>

#include <sched/common/Api.h>
#include <sched/common/Random.h>

#include "MutationTraits.h"

namespace sched::shop {

  struct SCHED_API SwapMutation {

    template<typename Input>
    Input operator()(const Input& input, Random& random) {
      assert(!input.empty());

      std::size_t max = input.size() - 1;
      std::size_t index0, index1;

      do {
        index0 = random.compute_uniform_integer(std::size_t{0}, max);
        index1 = random.compute_uniform_integer(std::size_t{0}, max);
      } while (index0 == index1 || input[index0] == input[index1]);

      Input mutant = input;
      std::swap(mutant[index0], mutant[index1]);
      return mutant;
    }

  };

  template<>
  struct MutationTraits<SwapMutation> {
    static std::string name() {
      return "swp";
    }
  };

}

#endif // SCHED_SWAP_MUTATION_H
