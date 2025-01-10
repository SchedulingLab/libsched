#ifndef SCHED_SHOP_GENETIC_ALGORITHM_H
#define SCHED_SHOP_GENETIC_ALGORITHM_H

#include <cassert>
#include <cmath>

#include <algorithm>

#include <sched/Instance.h>
#include <sched/criterion/Comparison.h>
#include <sched/meta/Solution.h>
#include <sched/support/Random.h>
#include <sched/support/Log.h>
#include <sched/types/InputTraits.h>
#include <sched/types/NeighborhoodTraits.h>

namespace sched::shop {

  template<typename Engine, typename Selection, typename Crossover, typename Mutation, typename Criterion>
  struct GeneticAlgorithm {
    using Input = typename Engine::Input;

    GeneticAlgorithm(double crossover, double mutation)
    : crossover_probability(crossover)
    , mutation_probability(mutation)
    {
    }

    template<typename Instance, typename Termination>
    auto operator()(const Instance& instance, Random& random, std::size_t population_size, Termination termination)
    {
      const LogScope log0;

      const std::size_t offsprings_size_max = population_size * 9 / 10;

      using Solution = Solution<Engine, Criterion, Instance>;

      auto compute_solution = [this, instance](Input&& input) {
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
        const LogScope log1;

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

          auto [child0, child1] = crossover(selected[i].input, selected[j].input, random);

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

          auto& solution = selected[i];
          auto mutant = mutation(solution.input, *solution.schedule, random);
          offsprings.push_back(compute_solution(std::move(mutant)));
        }

        // replacement

        if (offsprings.size() > offsprings_size_max) {
          std::sort(offsprings.begin(), offsprings.end(), solution_compare);
          offsprings.resize(offsprings_size_max);
        }

        offsprings.insert(offsprings.end(), population.begin(), population.end());
        assert(offsprings.size() >= population_size);
        offsprings.resize(population_size);
        std::sort(offsprings.begin(), offsprings.end(), solution_compare);
        population = std::move(offsprings);
        assert(std::is_sorted(population.begin(), population.end(), solution_compare));

        auto display_criterion = [this](auto value) {
          if (criterion.compare(value, Criterion::worst()) == Comparison::Equivalent) {
            using namespace std::literals;
            return "worst"s;
          }

          return fmt::format("{}", value);
        };

        // Log::println("#{}: {} with {} (last: {} with {})", generation, display_criterion(population.front().fitness), population.front().input, display_criterion(population.back().fitness), population.back().input);

        termination.step();
        ++generation;
      }

      return population;
    }

    static std::string input_name()
    {
      return InputTraits<Input>::name();
    }

    static std::string mutation_name()
    {
      return NeighborhoodTraits<Mutation>::name();
    }

    Engine engine;
    Selection selection;
    Crossover crossover;
    Mutation mutation;
    Criterion criterion;
    double crossover_probability;
    double mutation_probability;
  };

}

#endif // SCHED_SHOP_GENETIC_ALGORITHM_H
