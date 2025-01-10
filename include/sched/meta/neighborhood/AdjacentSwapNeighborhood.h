#ifndef SCHED_ADJACENT_SWAP_NEIGHBORHOOD_H
#define SCHED_ADJACENT_SWAP_NEIGHBORHOOD_H

#include <cassert>

#include <string>
#include <utility>

#include <sched/Api.h>
#include <sched/support/Random.h>
#include <sched/meta/neighborhood/NeighborhoodHelper.h>
#include <sched/types/InputConcepts.h>
#include <sched/types/NeighborhoodTraits.h>
#include <sched/meta/neighborhood/RowNeighborhood.h>

namespace sched {

  struct SCHED_API AdjacentSwapNeighborhood {

    template<concepts::Input Input, typename Schedule>
    Input operator()(const Input& input, [[maybe_unused]] const Schedule& schedule, Random& random)
    {
      assert(!input.empty());

      const std::size_t max = input.size() - 1;
      std::size_t index = 0;
      assert(index < input.size() - 1);

      while (input[index] == input[index + 1]) {
        index = random.compute_uniform_integer(std::size_t{ 0 }, max - 1);
        assert(index < input.size() - 1);
      }

      Input mutant = input;
      std::swap(mutant[index], mutant[index + 1]);
      return mutant;
    }

    template<concepts::Input Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      return generate_many_neighbors(*this, input, schedule, random, count);
    }

  };

  template<>
  struct NeighborhoodTraits<AdjacentSwapNeighborhood> {
    static std::string name()
    {
      return "adj";
    }
  };

  using AdjacentSwapMutation = AdjacentSwapNeighborhood;

  using AdjacentSwapInRowNeighborhood = RowNeighborhood<AdjacentSwapNeighborhood>;

}

#endif // SCHED_ADJACENT_SWAP_NEIGHBORHOOD_H
