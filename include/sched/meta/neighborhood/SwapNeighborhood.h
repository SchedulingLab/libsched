// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SWAP_NEIHGBORHOOD_H
#define SCHED_SWAP_NEIHGBORHOOD_H

#include <cassert>

#include <string>

#include <sched/Api.h>
#include <sched/support/Random.h>
#include <sched/meta/neighborhood/NeighborhoodHelper.h>
#include <sched/types/InputConcepts.h>
#include <sched/types/NeighborhoodTraits.h>
#include <sched/meta/neighborhood/RowNeighborhood.h>

namespace sched {

  struct SCHED_API SwapNeighborhood {

    template<concepts::Input Input, typename Schedule>
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

    template<concepts::Input Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      return generate_many_neighbors(*this, input, schedule, random, count);
    }

  };

  template<>
  struct NeighborhoodTraits<SwapNeighborhood> {
    static std::string name()
    {
      return "swp";
    }
  };

  using SwapMutation = SwapNeighborhood;

  using SwapInRowNeighborhood = RowNeighborhood<SwapNeighborhood>;

}

#endif // SCHED_SWAP_NEIHGBORHOOD_H
