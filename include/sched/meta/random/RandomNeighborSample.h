// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_RANDOM_NEIGHBOR_SAMPLE_H
#define SCHED_RANDOM_NEIGHBOR_SAMPLE_H

#include <cassert>

#include <vector>

#include <sched/support/Random.h>
#include <sched/Time.h>

namespace sched {

  template<typename Engine, typename Neighborhood>
  struct RandomNeighborSample {
    using Input = typename Engine::Input;

    template<typename Instance>
    std::vector<Input> operator()(const Instance& instance, const Input& start, Random& random, std::size_t count = 1000)
    {
      auto maybe_schedule = engine(instance, start);
      assert(maybe_schedule);
      auto schedule = *maybe_schedule;

      std::vector<Input> result;

      while (result.size() < count) {
        Input neighbor = neighborhood(start, schedule, random);

        if (engine(instance, neighbor)) {
          result.push_back(std::move(neighbor));
        }
      }

      return result;
    }

    Neighborhood neighborhood;
    Engine engine;
  };

}

#endif // SCHED_RANDOM_NEIGHBOR_SAMPLE_H
