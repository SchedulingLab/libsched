// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_GENETIC_ALGORITHM_H
#define SCHED_GENETIC_ALGORITHM_H

#include <cassert>
#include <cmath>

#include <algorithm>

#include <sched/meta/Instance.h>
#include <sched/criterion/Comparison.h>
#include <sched/meta/Solution.h>
#include <sched/support/Random.h>
#include <sched/support/Log.h>
#include <sched/types/InputTraits.h>

namespace sched {

  struct GeneticAlgorithmVisitor {

    template<typename Solution>
    void initial_population([[maybe_unused]] const std::vector<Solution>& population)
    {
    }

    template<typename Solution>
    void after_selection([[maybe_unused]] std::size_t generation, [[maybe_unused]] const std::vector<Solution>& population)
    {
    }

    template<typename Solution>
    void after_crossover([[maybe_unused]] std::size_t generation, [[maybe_unused]] const std::vector<Solution>& population)
    {
    }

    template<typename Solution>
    void after_mutation([[maybe_unused]] std::size_t generation, [[maybe_unused]] const std::vector<Solution>& population)
    {
    }

    template<typename Solution>
    void current_population([[maybe_unused]] std::size_t generation, [[maybe_unused]] const std::vector<Solution>& population)
    {
    }

  };

  template<typename Engine, typename Selection, typename Crossover, typename Mutation, typename Criterion, typename Visitor = GeneticAlgorithmVisitor>
  struct GeneticAlgorithm {
    using Input = typename Engine::Input;

    GeneticAlgorithm(double crossover, double mutation)
    : crossover_probability(crossover)
    , mutation_probability(mutation)
    {
    }

    using Solution = BasicSolution<Engine, Criterion>;

    template<typename Instance, typename Termination>
    auto operator()(const Instance& instance, Random& random, std::size_t population_size, Termination termination)
    {
      const LogScope log0;

      const std::size_t offsprings_size_max = population_size * 9 / 10;

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

      std::vector<Solution> population;

      for (std::size_t i = 0; i < population_size; ++i) {
        population.push_back(compute_solution(InputTraits<Input>::generate_feasible(instance, random)));
      }

      auto solution_compare = [this](const Solution& lhs, const Solution& rhs) {
        return criterion.compare(lhs.fitness, rhs.fitness) == Comparison::Better;
      };

      std::ranges::sort(population, solution_compare);

      visitor.initial_population(population);

      termination.start();
      std::size_t generation = 0;

      while (!termination.satisfied()) {
        const LogScope log1;

        // selection

        auto selected = compute_selection(population, population_size);
        visitor.after_selection(generation, selected);

        // crossover

        std::vector<Solution> offsprings = compute_crossover(selected, random);
        visitor.after_crossover(generation, offsprings);

        // mutation

        std::vector<Solution> offsprings_mutated = compute_mutation(selected, random);
        visitor.after_mutation(generation, offsprings_mutated);
        offsprings.insert(offsprings.end(), offsprings_mutated.begin(), offsprings_mutated.end());

        // replacement

        if (offsprings.size() > offsprings_size_max) {
          std::ranges::sort(offsprings, solution_compare);
          offsprings.resize(offsprings_size_max);
        }

        offsprings.insert(offsprings.end(), population.begin(), population.end());
        assert(offsprings.size() >= population_size);
        offsprings.resize(population_size);
        std::ranges::sort(offsprings, solution_compare);
        population = std::move(offsprings);
        assert(std::ranges::is_sorted(population, solution_compare));

        visitor.current_population(generation, population);

        termination.step();
        ++generation;
      }

      return population;
    }

    std::vector<Solution> compute_selection(const std::vector<Solution>& population, std::size_t population_size)
    {
      auto selected = selection(population, population_size / 2, random); // TODO: selection size
      assert(selected.size() > 1);
    }

    std::vector<Solution> compute_crossover(const std::vector<Solution>& selected, Random& random)
    {
      std::vector<Solution> offsprings;

      std::bernoulli_distribution crossover_distribution(crossover_probability);
      std::uniform_int_distribution<std::size_t> selected_distribution(0, selected.size() - 1);

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

      return offsprings;
    }

    std::vector<Solution> compute_mutation(const std::vector<Solution>& selected, Random& random)
    {
      std::vector<Solution> offsprings;

      std::bernoulli_distribution mutation_distribution(mutation_probability);

      for (std::size_t i = 0; i < selected.size(); ++i) {
        if (!mutation_distribution(random)) {
          continue;
        }

        auto& solution = selected[i];

        if (solution.schedule) {
          auto mutant = mutation(solution.input, *solution.schedule, random);
          offsprings.push_back(compute_solution(std::move(mutant)));
        }
      }

      return offsprings;
    }


    Engine engine;
    Selection selection;
    Crossover crossover;
    Mutation mutation;
    Criterion criterion;
    double crossover_probability;
    double mutation_probability;
    Visitor visitor;
  };

}

#endif // SCHED_GENETIC_ALGORITHM_H
