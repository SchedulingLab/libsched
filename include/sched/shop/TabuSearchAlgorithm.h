#ifndef SCHED_SHOP_TABU_SEARCH_ALGORITHM_H
#define SCHED_SHOP_TABU_SEARCH_ALGORITHM_H

#include <cmath>
#include <chrono>
#include <deque>
#include <optional>
#include <tuple>

#include <sched/common/Instance.h>
#include <sched/common/Random.h>
#include <sched/tools/Log.h>

#include "InputTraits.h"

namespace sched::shop {

  template<typename Engine, typename Neighborhood, typename Criterion>
  struct TabuSearchAlgorithm {
    using Input = typename Engine::Input;

    template<typename Instance, typename Duration>
    auto operator()(const Instance& instance, const Input& start, Random& random, std::size_t neighbors_count, Duration duration) {
      const std::size_t n = instance.job_count();
      const std::size_t m = instance.machine_count();

      std::size_t o = 0;

      for (auto job : jobs(instance)) {
        o += instance.operation_count(job);
      }

      const std::size_t tabu_duration = static_cast<std::size_t>((n + m / 2.0) * std::exp(- 1.0 * n / (5.0 * m)) + o / 2.0 * std::exp(-(5.0 * m) / n));

      Input best_input = start;
      auto best_schedule = *engine(instance, best_input);
      auto best_fitness = criterion(instance, best_schedule);

      Input current_input = best_input;
      auto current_schedule = best_schedule;
      auto current_fitness = best_fitness;

      std::size_t iteration = 0;

      struct Tabu {
        Input input;
        std::size_t iteration;
      };

      std::deque<Tabu> tabu_list;

      auto is_tabu = [&](const Input& input) {
        for (auto &tabu : tabu_list) {
          if (input == tabu.input) {
            return true;
          }
        }

        return false;
      };

      auto start_time = std::chrono::steady_clock::now();

      while ((std::chrono::steady_clock::now() - start_time) < duration) {
        std::optional<Input> candidate;
        decltype(current_schedule) candidate_schedule;
        decltype(current_fitness) candidate_fitness;

        auto neighbors = neighborhood.generate_many(current_input, current_schedule, random, neighbors_count);

        for (auto & neighbor_input : neighbors) {
          auto maybe_schedule = engine(instance, neighbor_input);

          if (!maybe_schedule) {
            continue;
          }

          auto neighbor_schedule = *maybe_schedule;
          auto neighbor_fitness = criterion(instance, neighbor_schedule);

          if (!candidate || criterion.is_strictly_better(candidate_fitness, neighbor_fitness)) {

            if (is_tabu(neighbor_input) && !criterion.is_strictly_better(current_fitness, neighbor_fitness)) {
              continue;
            }

            candidate = neighbor_input;
            candidate_schedule = neighbor_schedule;
            candidate_fitness = neighbor_fitness;
          }
        }

        while (!tabu_list.empty() && tabu_list.front().iteration + tabu_duration < iteration) {
          tabu_list.pop_front();
        }

        if (candidate) {
          current_input = *candidate;
          current_schedule = candidate_schedule;
          current_fitness = candidate_fitness;
          tabu_list.push_back({ std::move(*candidate), iteration });
        } else {
          tabu_list.clear();
          current_input = InputTraits<Input>::generate_feasible(instance, random);
          current_schedule = *engine(instance, current_input);
          current_fitness = criterion(instance, current_schedule);
        }

        if (criterion.is_strictly_better(best_fitness, current_fitness)) {
          best_input = current_input;
          best_schedule = current_schedule;
          best_fitness = current_fitness;
        }

        ++iteration;
      }

      return std::make_tuple(best_input, best_fitness, iteration);
    }

    Engine engine;
    Neighborhood neighborhood;
    Criterion criterion;
  };


}

#endif // SCHED_TABU_SEARCH_ALGORITHM_H
