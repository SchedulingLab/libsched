// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_HOIST_LOADED_ENGINE_H
#define SCHED_SHOP_HOIST_LOADED_ENGINE_H

#include <cassert>

#include <optional>

#include <lqp/GlpkSolver.h>
#include <lqp/Problem.h>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/shop/input/HoistLoadedInput.h>
#include <sched/shop/schedule/HoistSchedule.h>
#include <sched/types/EngineTraits.h>
#include <sched/types/ShopInstanceConcepts.h>
#include "sched/Time.h"

namespace sched::shop {

  struct SCHED_API HoistLoadedEngine {
    using Input = HoistLoadedInput;
    using Schedule = HoistSchedule;

    template<typename Instance>
      // requires(concepts::ShopInstance<Instance>)
    std::optional<HoistSchedule> operator()(const Instance& instance, const HoistLoadedInput& input)
    {
      using namespace sched::literals;

      assert(input.size() == instance.machine_count());
      const std::size_t machine_count = instance.machine_count();
      assert(machine_count >= 2);

      const std::vector<EmptyMove> empty_moves = compute_empty_moves(input);

      lqp::Problem problem;
      constexpr double Infinity = 1e6;

      /*
       * variables
       */

      lqp::VariableId period = problem.add_variable(lqp::VariableCategory::Integer, "period");

      std::vector<lqp::VariableId> b_i;
      b_i.reserve(machine_count);

      for (std::size_t i = 0; i < machine_count; ++i) {
        b_i.push_back(problem.add_variable(lqp::VariableCategory::Binary, "b_" + std::to_string(i + 1)));
      }

      std::vector<lqp::VariableId> t_i;
      t_i.reserve(machine_count);

      for (std::size_t i = 0; i < machine_count; ++i) {
        t_i.push_back(problem.add_variable(lqp::VariableCategory::Integer, lqp::lower_bound(0), "t_" + std::to_string(i + 1)));
      }

      /*
       * constraints
       */

      problem.add_constraint(t_i[1] == instance.transportation_time_loaded(0_m, 1_m));

      for (const EmptyMove move : empty_moves) {
        problem.add_constraint(t_i[move.orig] + instance.transportation_time_empty(move.orig, move.dest) <= period);
      }

      for (std::size_t i = 0; i < machine_count; ++i) {
        const std::size_t j = (i + 1) % machine_count;

        problem.add_constraint((b_i[i] - 1) * Infinity <= t_i[i] - t_i[j]);
        problem.add_constraint(t_i[i] - t_i[j] <= b_i[i] * Infinity);
      }

      for (std::size_t i = 0; i < machine_count; ++i) {
        const std::size_t next_i = (i + 1) % machine_count;
        const Time r_i = instance.transportation_time_loaded(sched::machine(i), sched::machine(next_i));

        const OperationId operation = sched::operation(sched::AnyJob, i);
        const TimeWindow time_window = instance.processing_time(operation, sched::machine(i));

        problem.add_constraint(-b_i[i] * Infinity + double(time_window.lo) <= (t_i[next_i] - r_i) - t_i[i]);

        if (time_window.hi != sched::TimeMax) {
          problem.add_constraint((t_i[next_i] - r_i) - t_i[i] <= double(time_window.hi) + b_i[i] * Infinity);
        }

        problem.add_constraint((b_i[i] - 1) * Infinity + double(time_window.lo) <= period + (t_i[next_i] - r_i) - t_i[i]);

        if (time_window.hi != sched::TimeMax) {
          problem.add_constraint(period + (t_i[next_i] - r_i) - t_i[i] <= double(time_window.hi) + (1 - b_i[i]) * Infinity);
        }
      }

      for (const EmptyMove move : empty_moves) {
        const MachineId i = move.orig;
        const MachineId j = move.dest;
        const Time d_ij = instance.transportation_time_empty(i, j);
        const std::size_t next_j = (to_index(j) + 1) % machine_count;
        const Time r_j = instance.transportation_time_loaded(j, sched::machine(next_j));

        if (j == 0_m) {
          problem.add_constraint(t_i[to_index(i)] + d_ij <= period + t_i[next_j] - r_j);
        } else {
          problem.add_constraint(t_i[to_index(i)] + d_ij <= t_i[next_j] - r_j);
        }
      }

      /*
       * objective
       */

      problem.set_objective(lqp::Sense::Minimize, period, "objective");

      /*
       * solve
       */

      lqp::GlpkSolver solver;
      assert(solver.available());

      lqp::SolverConfig config;
      config.use_mip = true;
      config.verbose = false;
      config.presolve = true;
      //         config.problem_output = "problem.txt";
      //         config.solution_output = "solution.txt";

      const lqp::Solution solution = solver.solve(problem, config);

      if (solution.status() != lqp::SolutionStatus::Optimal) {
        return std::nullopt;
      }

      HoistSchedule schedule;

      schedule.period = static_cast<sched::Time>(solution.value(period));

      for (std::size_t i = 0; i < machine_count; ++i) {
        const std::size_t next_i = (i + 1) % machine_count;
        const Time r_i = instance.transportation_time_loaded(sched::machine(i), sched::machine(next_i));

        HoistSchedulingTask task = {};
        task.operation = operation(sched::AnyJob, i);
        task.start = static_cast<sched::Time>(solution.value(t_i[i]));
        task.completion = static_cast<sched::Time>(solution.value(t_i[next_i])) - r_i;

        if (task.completion < 0) {
          task.completion += schedule.period;
        }

        schedule.append(task);
      }

      return schedule;
    }

  private:
    struct EmptyMove {
      MachineId orig;
      MachineId dest;
    };

    static std::vector<EmptyMove> compute_empty_moves(const HoistLoadedInput& input)
    {
      std::vector<EmptyMove> moves;

      for (std::size_t i = 0; i < input.size(); ++i) {
        EmptyMove move = {};
        move.orig = machine(((input[i] + 1)) % input.size());
        move.dest = input[(i + 1) % input.size()];
        moves.push_back(move);
      }

      return moves;
    }
  };

}

namespace sched {

  template<>
  struct EngineTraits<shop::HoistLoadedEngine> {
    static std::string name()
    {
      using namespace std::literals;
      return "ldd";
    }
  };

}

#endif // SCHED_SHOP_HOIST_LOADED_ENGINE_H
