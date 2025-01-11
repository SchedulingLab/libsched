// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SIMPLE_DESCENT_H
#define SCHED_SIMPLE_DESCENT_H

#include <sched/criterion/Comparison.h>
#include <sched/support/Random.h>
#include <sched/meta/Solution.h>
#include <sched/types/InputTraits.h>

namespace sched {

  template<typename Engine, typename Neighborhood, typename Criterion>
  struct SimpleDescent {
    using Input = typename Engine::Input;

    template<typename Instance, typename Termination>
    auto operator()(const Instance& instance, const Input& start, Random& random, std::size_t neighbors_count, Termination termination)
    {
      using Solution = Solution<Engine, Criterion, Instance>;

      Solution best;
      best.input = start;

      for (;;) {
        best.schedule = engine(instance, best.input);

        if (best.schedule) {
          best.fitness = criterion(instance, *best.schedule);
          break;
        }

        best.input = InputTraits<Input>::generate_feasible(instance, random);
      }

      Solution current = best;

      //       std::cout << "\t- " << current << ": " << best_fitness << '\n';

      termination.start();

      while (!termination.satisfied()) {
        //         std::cout << "Iteration #" << i << '\n';

        auto neighbors = neighborhood.generate_many(current.input, *current.schedule, random, neighbors_count);


        for (auto& neighbor_input : neighbors) {
          Solution neighbor;
          neighbor.input = std::move(neighbor_input);
          neighbor.schedule = engine(instance, neighbor.input);

          if (!neighbor.schedule) {
            continue;
          }

          neighbor.fitness = criterion(instance, *neighbor.schedule);

          //           std::cout << "\t* " << neighbor << ": " << fitness;

          if (criterion.compare(neighbor.fitness, current.fitness) == Comparison::Better) {
            current = neighbor;
            //             std::cout << " KEEP";
          }

          //           std::cout << '\n';
        }

        if (criterion.compare(current.fitness, best.fitness) == Comparison::Better) {
          best = current;
          //             std::cout << " KEEP";
        }

        termination.step();
      }

      return best;
    }

    Neighborhood neighborhood;
    Engine engine;
    Criterion criterion;
  };

}

#endif // SCHED_SIMPLE_DESCENT_H
