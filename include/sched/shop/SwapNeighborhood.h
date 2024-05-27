#ifndef SCHED_SWAP_NEIHGBORHOOD_H
#define SCHED_SWAP_NEIHGBORHOOD_H

#include <cassert>

#include <string>

#include <sched/common/Api.h>
#include <sched/common/Random.h>

#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"

namespace sched::shop {

  struct SCHED_API SwapNeighborhood {

    template<typename Input, typename Schedule>
    Input operator()(const Input& input, [[maybe_unused]] const Schedule& schedule, Random& random)
    {
      assert(!input.empty());

      std::size_t max = input.size() - 1;
      std::size_t index0 = 0;
      std::size_t index1 = 0;

      while (index0 == index1 || input[index0] == input[index1]) {
        index0 = random.compute_uniform_integer(std::size_t{ 0 }, max);
        index1 = random.compute_uniform_integer(std::size_t{ 0 }, max);
      }

      Input mutant = input;
      std::swap(mutant[index0], mutant[index1]);
      return mutant;
    }

    template<typename Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

  };

  template<>
  struct NeighborhoodTraits<SwapNeighborhood> {
    static std::string name()
    {
      return "swp";
    }
  };

}

#endif // SCHED_SWAP_NEIHGBORHOOD_H
