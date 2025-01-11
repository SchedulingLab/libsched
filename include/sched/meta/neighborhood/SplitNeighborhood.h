// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SPLIT_NEIGHBORHOOD_H
#define SCHED_SPLIT_NEIGHBORHOOD_H

#include <cassert>
#include <cstdint>

#include <string>
#include <vector>

#include <sched/meta/input/SplitInput.h>
#include <sched/meta/neighborhood/NeighborhoodHelper.h>
#include <sched/support/Random.h>
#include <sched/types/InputConcepts.h>
#include <sched/types/NeighborhoodTraits.h>
#include <sched/types/NeighborhoodConcepts.h>

namespace sched {

  enum class SplitNeighborhoodKind : uint8_t {
    One,
    All,
  };

  template<SplitNeighborhoodKind Kind, typename Neighborhood0, typename Neighborhood1>
  struct SplitNeighborhood {

    template<concepts::Input Input0, concepts::Input Input1, typename Schedule>
    SplitInput<Input0, Input1> operator()(const SplitInput<Input0, Input1>& input, const Schedule& schedule, Random& random)
    {
      static_assert(concepts::NeighborhoodFor<Neighborhood0, Input0, Schedule>);
      static_assert(concepts::NeighborhoodFor<Neighborhood1, Input1, Schedule>);

      SplitInput<Input0, Input1> neighbor = input;

      if constexpr (Kind == SplitNeighborhoodKind::One) {
        std::bernoulli_distribution dist(0.5);

        if (dist(random)) {
          neighbor.assignment = neighborhood0(neighbor.assignment, schedule, random);
        } else {
          neighbor.schedule = neighborhood1(neighbor.schedule, schedule, random);
        }
      } else {
        neighbor.assignment = neighborhood0(neighbor.assignment, schedule, random);
        neighbor.schedule = neighborhood1(neighbor.schedule, schedule, random);
      }

      return neighbor;
    }

    template<concepts::Input Input0, concepts::Input Input1, typename Schedule>
    std::vector<SplitInput<Input0, Input1>> generate_many(const SplitInput<Input0, Input1>& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      static_assert(concepts::NeighborhoodFor<Neighborhood0, Input0, Schedule>);
      static_assert(concepts::NeighborhoodFor<Neighborhood1, Input1, Schedule>);

      auto assignment_neighbors = neighborhood0.generate_many(input.assignment, schedule, random, count);
      assert(assignment_neighbors.size() == count);
      auto schedule_neighbors = neighborhood1.generate_many(input.schedule, schedule, random, count);
      assert(schedule_neighbors.size() == count);

      std::vector<SplitInput<Input0, Input1>> neighbors;

      for (std::size_t i = 0; i < count; ++i) {
        if constexpr (Kind == SplitNeighborhoodKind::One) {
          std::bernoulli_distribution dist(0.5);

          if (dist(random)) {
            neighbors.push_back({ std::move(assignment_neighbors[i]), input.schedule });
          } else {
            neighbors.push_back({ input.assignment, std::move(schedule_neighbors[i]) });
          }
        } else {
          neighbors.push_back({ std::move(assignment_neighbors[i]), std::move(schedule_neighbors[i]) });
        }
      }

      return neighbors;
    }

    Neighborhood0 neighborhood0;
    Neighborhood1 neighborhood1;
  };

  template<SplitNeighborhoodKind Kind, typename Neighborhood0, typename Neighborhood1>
  struct NeighborhoodTraits<SplitNeighborhood<Kind, Neighborhood0, Neighborhood1>> {
    static std::string name()
    {
      return NeighborhoodTraits<Neighborhood0>::name() + '_' + NeighborhoodTraits<Neighborhood1>::name() + '-' + (Kind == SplitNeighborhoodKind::One ? "one" : "all");
    }
  };

}

#endif // SCHED_SPLIT_NEIGHBORHOOD_H
