// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_ALTERNATE_NEIGHBORHOOD_H
#define SCHED_ALTERNATE_NEIGHBORHOOD_H

#include <cassert>

#include <string>

#include <sched/support/Random.h>
#include <sched/types/InputConcepts.h>
#include <sched/types/NeighborhoodTraits.h>

namespace sched {

  template<typename Neighborhood0, typename Neighborhood1>
  struct AlternateNeighborhood {

    template<concepts::Input Input, typename Schedule>
    Input operator()(const Input& input, const Schedule& schedule, Random& random)
    {
      std::bernoulli_distribution dist(0.5);
      Input neighbor;

      if (dist(random)) {
        neighbor = neighborhood0(input, schedule, random);
      } else {
        neighbor = neighborhood1(input, schedule, random);
      }

      return neighbor;
    }

    template<concepts::Input Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      auto neighbors0 = neighborhood0.generate_many(input, schedule, random, count);
      assert(neighbors0.size() == count);
      auto neighbors1 = neighborhood1.generate_many(input, schedule, random, count);
      assert(neighbors1.size() == count);

      std::bernoulli_distribution dist(0.5);
      std::vector<Input> neighbors;

      for (std::size_t i = 0; i < count; ++i) {
        if (dist(random)) {
          neighbors.push_back(std::move(neighbors0[i]));
        } else {
          neighbors.push_back(std::move(neighbors1[i]));
        }
      }

      return neighbors;
    }

    Neighborhood0 neighborhood0;
    Neighborhood1 neighborhood1;
  };

  template<typename Neighborhood0, typename Neighborhood1>
  struct NeighborhoodTraits<AlternateNeighborhood<Neighborhood0, Neighborhood1>> {
    static std::string name()
    {
      return NeighborhoodTraits<Neighborhood0>::name() + "_or_" + NeighborhoodTraits<Neighborhood1>::name();
    }
  };

  template<typename Neighborhood0, typename Neighborhood1>
  using AlternateMutation = AlternateNeighborhood<Neighborhood0, Neighborhood1>;

}

#endif // SCHED_ALTERNATE_NEIGHBORHOOD_H
