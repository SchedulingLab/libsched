// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_RANDOM_WALK_H
#define SCHED_RANDOM_WALK_H

#include <cassert>

#include <vector>

#include <sched/support/Random.h>
#include <sched/Time.h>

namespace sched {

  template<typename Engine, typename Neighborhood, typename Criterion>
  struct RandomWalk {
    using Input = typename Engine::Input;
    using Fitness = typename Criterion::Fitness;

    template<typename Instance>
    std::vector<Fitness> operator()(const Instance& instance, const Input& start, Random& random, std::size_t count = 1000)
    {
      Input current = start;

      auto maybe_schedule = engine(instance, current);
      assert(maybe_schedule);
      auto schedule = *maybe_schedule;

      std::vector<Fitness> fitness;
      fitness.push_back(criterion(instance, schedule));

      for (std::size_t i = 0; i < count; ++i) {
        for (;;) {
          auto next = neighborhood(current, schedule, random);
          auto maybe_next_schedule = engine(instance, next);

          if (!maybe_next_schedule) {
            continue;
          }

          current = next;
          schedule = *maybe_next_schedule;
          fitness.push_back(criterion(instance, schedule));
          break;
        }
      }

      return fitness;
    }

    Neighborhood neighborhood;
    Engine engine;
    Criterion criterion;
  };

}

#endif // SCHED_RANDOM_WALK_H
