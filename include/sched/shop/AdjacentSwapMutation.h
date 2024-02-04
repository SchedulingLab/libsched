#ifndef SCHED_SHOP_ADJACENT_SWAP_MUTATION_H
#define SCHED_SHOP_ADJACENT_SWAP_MUTATION_H

#include <cassert>

#include <string>

#include <sched/common/Api.h>
#include <sched/common/Random.h>

#include "MutationTraits.h"

namespace sched::shop {

  struct SCHED_API AdjacentSwapMutation {

    template<typename Input>
    Input operator()(const Input& input, Random& random)
    {
      assert(!input.empty());

      const std::size_t max = input.size() - 1;
      std::size_t index = 0;

      // NOLINTNEXTLINE(cppcoreguidelines-avoid-do-while)
      do {
        index = random.compute_uniform_integer(std::size_t{ 0 }, max - 1);
        assert(index < input.size() - 1);
      } while (input[index] == input[index + 1]);

      Input mutant = input;
      std::swap(mutant[index], mutant[index + 1]);
      return mutant;
    }
  };

  template<>
  struct MutationTraits<AdjacentSwapMutation> {
    static std::string name()
    {
      return "adj";
    }
  };

}

#endif // SCHED_SHOP_ADJACENT_SWAP_MUTATION_H
