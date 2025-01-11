// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_NEIGHBORHOOD_HELPER_H
#define SCHED_NEIGHBORHOOD_HELPER_H

#include <vector>

#include <sched/Api.h>
#include <sched/support/Random.h>
#include <sched/types/NeighborhoodConcepts.h>

namespace sched {

  template<typename Neighborhood, typename Input, typename Schedule>
  std::vector<Input> generate_many_neighbors(Neighborhood& neighborhood, const Input& input, const Schedule& schedule, Random& random, std::size_t count)
  {
    static_assert(concepts::NeighborhoodFor<Neighborhood, Input, Schedule>);

    std::vector<Input> result;

    for (std::size_t i = 0; i < count; ++i) {
      result.push_back(neighborhood(input, schedule, random));
    }

    return result;
  }

}

#endif // SCHED_NEIGHBORHOOD_HELPER_H
