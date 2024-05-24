#ifndef SCHED_SHOP_TABU_SEARCH_ALGORITHM_H
#define SCHED_SHOP_TABU_SEARCH_ALGORITHM_H

#include <cmath>

#include <algorithm>
#include <deque>
#include <optional>
#include <tuple>

#include <sched/common/Comparison.h>
#include <sched/common/Instance.h>
#include <sched/common/Random.h>
#include <sched/tools/Log.h>

#include "InputSize.h"
#include "InputTraits.h"
#include "NeighborhoodTraits.h"
#include "Solution.h"

namespace sched::shop {

  template<typename Engine, typename Neighborhood, typename Criterion>
  struct TabuSearchAlgorithm {
    using Input = typename Engine::Input;

    template<typename Instance, typename Termination>
    auto operator()(const Instance& instance, const Input& start, Random& random, std::size_t neighbors_count, Termination termination)
    {
      const std::size_t n = instance.job_count();
      const std::size_t m = instance.machine_count();

      auto operation_count = input_size_for(instance);
      auto tabu_duration = static_cast<std::size_t>((n + m / 2.0) * std::exp(-1.0 * n / (5.0 * m)) + operation_count / 2.0 * std::exp(-(5.0 * m) / n));

      using Solution = Solution<Engine, Criterion, Instance>;

      Solution best;
      best.input = start;

      for (;;) {
        auto maybe_schedule = engine(instance, best.input);

        if (maybe_schedule) {
          best.schedule = *maybe_schedule;
          best.fitness = criterion(instance, best.schedule);
          break;
        }

        best.input = InputTraits<Input>::generate_feasible(instance, random);
      }

      Solution current = best;
      std::size_t iteration = 0;

      struct Tabu {
        Input input;
        std::size_t iteration;
      };

      std::deque<Tabu> tabu_list;

      auto is_tabu = [&](const Input& input) {
        return std::find_if(tabu_list.begin(), tabu_list.end(), [&](const Tabu& tabu) { return tabu.input == input; }) != tabu_list.end();
      };

      termination.start();

      while (!termination.satisfied()) {
        bool has_candidate = false;
        Solution candidate;

        auto neighbors = neighborhood.generate_many(current.input, current.schedule, random, neighbors_count);

        Solution neighbor;

        for (auto& neighbor_input : neighbors) {
          auto maybe_schedule = engine(instance, neighbor_input);

          if (!maybe_schedule) {
            continue;
          }

          neighbor.input = neighbor_input;
          neighbor.schedule = *std::move(maybe_schedule);
          neighbor.fitness = criterion(instance, neighbor.schedule);

          if (!has_candidate || criterion.compare(neighbor.fitness, candidate.fitness) == Comparison::Better) {

            if (is_tabu(neighbor.input) && criterion.compare(neighbor.fitness, current.fitness) != Comparison::Better) {
              continue;
            }

            candidate = neighbor;
            has_candidate = true;
          }
        }

        while (!tabu_list.empty() && tabu_list.front().iteration + tabu_duration < iteration) {
          tabu_list.pop_front();
        }

        if (has_candidate) {
          current = candidate;
          tabu_list.push_back({ std::move(*candidate.input), iteration });
        } else {
          tabu_list.clear();

          for (;;) {
            current.input = InputTraits<Input>::generate_feasible(instance, random);
            auto maybe_schedule = engine(instance, current.input);

            if (maybe_schedule) {
              current.schedule = *std::move(maybe_schedule);
              current.fitness = criterion(instance, current.schedule);
              break;
            }
          }
        }

        if (criterion.compare(current.fitness, best.fitness) == Comparison::Better) {
          best = current;
        }

        ++iteration;
        termination.step();
      }

      return std::make_tuple(best.input, best.fitness, best.schedule, iteration);
    }

    static std::string input_name()
    {
      return InputTraits<Input>::name();
    }

    static std::string neighborhood_name()
    {
      return NeighborhoodTraits<Neighborhood>::name();
    }

    Engine engine;
    Neighborhood neighborhood;
    Criterion criterion;
  };

}

#endif // SCHED_SHOP_TABU_SEARCH_ALGORITHM_H
