#ifndef SCHED_SHOP_GENETIC_ALGORITHM_H
#define SCHED_SHOP_GENETIC_ALGORITHM_H

#include <cmath>
#include <chrono>
#include <deque>
#include <optional>
#include <tuple>

#include <sched/common/Comparison.h>
#include <sched/common/Instance.h>
#include <sched/common/Random.h>
#include <sched/tools/Log.h>

#include "InputTraits.h"
#include "MutationTraits.h"
#include "Solution.h"

namespace sched::shop {

  template<typename Engine, typename Selection, typename Mutation, typename Crossover, typename Criterion>
  struct GeneticAlgorithm {
    using Input = typename Engine::Input;

    GeneticAlgorithm(double crossover, double mutation)
    : crossover_probability(crossover)
    , mutation_probability(mutation)
    {
    }

    template<typename Instance, typename Termination>
    auto operator()(const Instance& instance, Random& random, std::size_t population_size, Termination termination) {
      LogScope log0;

      using Solution = Solution<Engine, Criterion, Instance>;

      auto compute_solution = [this,instance](Input&& input) {
        Solution solution;
        solution.input = std::move(input);
        solution.schedule = engine(instance, solution.input);

        if (solution.schedule) {
          solution.fitness = criterion(instance, *solution.schedule);
        } else {
          solution.fitness = Criterion::worst();
        }

        return solution;
      };

      // initial population

//       Log::println("Initial population");

      std::vector<Solution> population;

      for (std::size_t i = 0; i < population_size; ++i) {
        population.push_back(compute_solution(InputTraits<Input>::generate_feasible(instance, random)));
      }

      auto solution_compare = [this](const Solution& lhs, const Solution& rhs) {
        return criterion.compare(lhs.fitness, rhs.fitness) == Comparison::Better;
      };

      std::sort(population.begin(), population.end(), solution_compare);

      termination.start();
      std::size_t generation = 0;

      while (!termination.satisfied()) {
        LogScope log1;

        // selection

//         Log::println("Selection");

        auto selected = selection(population, population_size / 2, random); // TODO: selection size
        assert(selected.size() > 1);

        std::uniform_int_distribution<std::size_t> selected_distribution(0, selected.size() - 1);

        // offsprings

        std::vector<Solution> offsprings;

        // crossover

//         Log::println("Crossover");

        std::bernoulli_distribution crossover_distribution(crossover_probability);

        for (std::size_t i = 0; i < selected.size(); ++i) {
          if (!crossover_distribution(random)) {
            continue;
          }

          std::size_t j = selected_distribution(random);

          while (j == i) {
            j = selected_distribution(random);
          }

          auto [ child0, child1 ] = crossover(selected[i].input, selected[j].input, random);

          offsprings.push_back(compute_solution(std::move(child0)));
          offsprings.push_back(compute_solution(std::move(child1)));
        }

        // mutation

//         Log::println("Mutation");

        std::bernoulli_distribution mutation_distribution(mutation_probability);

        for (std::size_t i = 0; i < selected.size(); ++i) {
          if (!mutation_distribution(random)) {
            continue;
          }

          auto & solution = selected[i];
          auto mutant = mutation(solution.input, random);
          offsprings.push_back(compute_solution(std::move(mutant)));
        }

        // replacement

        offsprings.insert(offsprings.end(), population.begin(), population.end());
        std::sort(offsprings.begin(), offsprings.end(), solution_compare);
        offsprings.resize(population_size);
        std::swap(offsprings, population);

        if (criterion.compare(population.front().fitness, Criterion::worst()) == Comparison::Equivalent) {
          Log::println("#{}: worst with {}", generation, population.front().input);
        } else {
          Log::println("#{}: {} with {}", generation, population.front().fitness, population.front().input);
        }

        termination.step();
        ++generation;
      }

      return population;
    }

    static std::string input_name() {
      return InputTraits<Input>::name();
    }

    static std::string mutation_name() {
      return MutationTraits<Mutation>::name();
    }

    Engine engine;
    Selection selection;
    Mutation mutation;
    Crossover crossover;
    Criterion criterion;
    double crossover_probability;
    double mutation_probability;
  };


}

#endif // SCHED_SHOP_GENETIC_ALGORITHM_H
