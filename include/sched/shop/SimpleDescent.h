#ifndef SCHED_SIMPLE_DESCENT_H
#define SCHED_SIMPLE_DESCENT_H

#include <sched/common/Random.h>

#include "JobShopSchedule.h"

namespace sched::shop {

  template<typename Engine, typename Neighborhood, typename Criterion>
  struct SimpleDescent {
    using Input = typename Engine::Input;

    SimpleDescent(int iterations = 10)
    : iterations(iterations)
    {
    }

    template<typename Instance>
    JobShopSchedule operator()(const Instance& instance, const Input& input, Random& random) {
      // find a feasible input
      Input current = find_feasible(instance, input, random);

      JobShopSchedule best_schedule = *engine(instance, current);
      int64_t best_fitness = criterion(instance, best_schedule);

//       std::cout << "\t- " << current << ": " << best_fitness << '\n';

      for (int i = 0; i < iterations; ++i) {
//         std::cout << "Iteration #" << i << '\n';

        auto neighbors = neighborhood(current, random);

        for (auto neighbor : neighbors) {
          auto sched = engine(instance, neighbor);

          if (!sched) {
            continue;
          }

          auto fitness = criterion(instance, *sched);

//           std::cout << "\t* " << neighbor << ": " << fitness;

          if (fitness < best_fitness) {
            best_fitness = fitness;
            best_schedule = *sched;
            current = neighbor;

//             std::cout << " KEEP";
          }

//           std::cout << '\n';

          // TODO: keep a similar neighbor or random
        }
      }

      return best_schedule;
    }

    template<typename Instance>
    Input find_feasible(const Instance& instance, const Input& input, Random& random) {
      Input current = input;

      while (!engine(instance, current)) {
        auto neighbors = neighborhood(current, random);

        while (neighbors.empty()) {
          neighbors = neighborhood(current, random);
        }

        current = neighbors.front();
      }

      return current;
    }

    int iterations;
    Neighborhood neighborhood;
    Engine engine;
    Criterion criterion;
  };

}

#endif // SCHED_SIMPLE_DESCENT_H
