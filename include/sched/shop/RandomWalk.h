#ifndef SCHED_RANDOM_WALK_H
#define SCHED_RANDOM_WALK_H

#include <cassert>
#include <vector>

#include <sched/common/Random.h>
#include <sched/common/Time.h>
#include <sched/tools/Statistics.h>

#include "JobShopSchedule.h"

namespace sched::shop {

  template<typename Engine, typename Neighborhood, typename Criterion>
  struct RandomWalk {
    using Input = typename Engine::Input;
    using Fitness = typename Criterion::Fitness;

    template<typename Instance>
    std::vector<Fitness> operator()(const Instance& instance, const Input& start, Random& random, std::size_t count = 1000) {
      // find a first feasible solution
      Input current = start;

      assert(engine(instance, current));

      std::vector<Fitness> fitness;
      JobShopSchedule schedule = *engine(instance, current);
      fitness.push_back(criterion(instance, schedule));

      for (std::size_t i = 0; i < count; ++i) {
        Input next;

//         std::cout << '.' << std::flush;

        do {
          next = neighborhood(current, random);
        } while (!engine(instance, next));

        assert(neighborhood.are_neighbors(current, next));

        current = next;

        schedule = *engine(instance, current);
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
