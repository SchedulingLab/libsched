#ifndef SCHED_RANDOM_WALK_H
#define SCHED_RANDOM_WALK_H

#include <cassert>

#include <vector>

#include <sched/common/Random.h>
#include <sched/common/Time.h>

namespace sched::shop {

  template<typename Engine, typename Neighborhood, typename Criterion>
  struct RandomWalk {
    using Input = typename Engine::Input;
    using Fitness = typename Criterion::Fitness;

    template<typename Instance>
    std::vector<Fitness> operator()(const Instance& instance, const Input& start, Random& random, std::size_t count = 1000)
    {
      // find a first feasible solution
      Input current = start;

      auto maybe_schedule = engine(instance, current);
      assert(maybe_schedule);
      auto schedule = *maybe_schedule;

      std::vector<Fitness> fitness;
      fitness.push_back(criterion(instance, schedule));

      for (std::size_t i = 0; i < count; ++i) {
        Input next;
        decltype(maybe_schedule) maybe_next_schedule;

        do {
          next = neighborhood(current, schedule, random);
          maybe_next_schedule = engine(instance, next);
        } while (!maybe_next_schedule);

        current = next;
        schedule = *maybe_next_schedule;
        fitness.push_back(criterion(instance, schedule));
      }

      return fitness;
    }

    Neighborhood neighborhood;
    Engine engine;
    Criterion criterion;
  };

}

#endif // SCHED_RANDOM_WALK_H
