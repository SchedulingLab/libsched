// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/algorithm/JobShopOptimalAlgorithm.h>

#include <lqp/Problem.h>

#include <sched/meta/Instance.h>
#include "lqp/Solution.h"

namespace sched::shop {

  JobShopSchedule JobShopOptimalAlgorithm::operator()(const JobShopInstance& instance, lqp::Solver& solver)
  {
    lqp::Problem problem;

    /*
     * variables:
     * - t_q: start time of operation q
     * - z_q_r_k: binary variable, 1 if operation q precedes operation r on machine k
     * - c_max: makespan of the schedule
     */

    std::map<OperationId, lqp::VariableId> times;

    for (auto job : jobs(instance)) {
      for (auto operation : operations(instance, job)) {
        times[operation] = problem.add_variable(lqp::VariableCategory::Integer, lqp::lower_bound(0.0), "t_" + to_string(operation));
      }
    }

    std::map<MachineId, std::vector<OperationId>> operation_by_machines;

    for (auto job : jobs(instance)) {
      for (std::size_t i = 0; i < instance.operation_count(job); ++i) {
        const OperationId operation = { .job = job, .index = i };
        const MachineId machine = instance.assigned_machine_for_operation(operation);
        operation_by_machines[machine].push_back(operation);
      }
    }

    std::map<std::tuple<OperationId, OperationId>, lqp::VariableId> precedence;

    for (auto& [ machine, operations ] : operation_by_machines) {
      const std::size_t count = operations.size();

      for (std::size_t i = 0; i < count; ++i) {
        const OperationId op1 = operations[i];
        const std::string op1_name = to_string(op1);

        for (std::size_t j = i + 1; j < count; ++j) {
          const OperationId op2 = operations[j];
          const std::string op2_name = to_string(op2);

          std::string name = "z_";
          name += op1_name;
          name += '_';
          name += op2_name;

          precedence[std::make_tuple(op1, op2)] = problem.add_variable(lqp::VariableCategory::Binary, std::move(name));
        }
      }
    }

    const lqp::VariableId cmax = problem.add_variable(lqp::VariableCategory::Integer, lqp::lower_bound(0.0), "cmax");

    /*
     * constraints:
     * - t_q + p_q <= t_r, for all operations r that follow q in a job
     * - t_q >= t_r + p_r - M * z_q_r_k
     * - t_r >= t_q + p_q - M * (1 - z_q_r_k)
     * - t_q + p_q <= c_max, for all operations
     */

    for (auto job : jobs(instance)) {
      for (std::size_t i = 1; i < instance.operation_count(job); ++i) {
        const OperationId current = { .job = job, .index = i };
        const OperationId previous = { .job = job, .index = i - 1 };
        const MachineId machine = instance.assigned_machine_for_operation(previous);
        const auto processing_time = static_cast<double>(instance.processing_time(previous, machine));

        problem.add_constraint(times[previous] + processing_time <= times[current], "precedence_" + to_string(previous));
      }
    }

    Time big_constant = 0;

    for (auto job : jobs(instance)) {
      for (auto operation : operations(instance, job)) {
        const MachineId machine = instance.assigned_machine_for_operation(operation);
        big_constant += instance.processing_time(operation, machine);
      }
    }

    for (auto& [machine, operations] : operation_by_machines) {
      const std::size_t count = operations.size();

      for (std::size_t i = 0; i < count; ++i) {
        const OperationId op1 = operations[i];
        const auto p1 = static_cast<double>(instance.processing_time(op1, machine));

        for (std::size_t j = i + 1; j < count; ++j) {
          const OperationId op2 = operations[j];
          const auto p2 = static_cast<double>(instance.processing_time(op2, machine));

          std::string name = "disjunction_";
          name += to_string(op1);
          name += '_';
          name += to_string(op2);

          problem.add_constraint(times[op1] >= times[op2] + p2 - double(big_constant) * precedence[std::make_tuple(op1, op2)], name + "_1");
          problem.add_constraint(times[op2] >= times[op1] + p1 - double(big_constant) * (1 - precedence[std::make_tuple(op1, op2)]), name + "2");
        }
      }
    }

    for (auto job : jobs(instance)) {
      for (auto operation : operations(instance, job)) {
        const MachineId machine = instance.assigned_machine_for_operation(operation);
        const auto processing_time = static_cast<double>(instance.processing_time(operation, machine));

        problem.add_constraint(times[operation] + processing_time <= cmax, "cmax_" + to_string(operation));
      }
    }

    /*
     * objective
     */

    problem.set_objective(lqp::Sense::Minimize, cmax, "cmax");

    lqp::SolverConfig config;
    config.use_mip = true;
    config.presolve = true;
    config.verbose = false;

    auto solution = solver.solve(problem, config);

    JobShopSchedule schedule;

    if (solution.status() != lqp::SolutionStatus::Optimal) {
      return schedule;
    };

    for (auto job : jobs(instance)) {
      for (auto operation : operations(instance, job)) {
        JobShopTask task = {};

        task.operation = operation;
        task.machine = instance.assigned_machine_for_operation(operation);
        task.start = Time(solution.value(times[operation]));
        task.completion = task.start + instance.processing_time(operation, task.machine);

        schedule.append(task);
      }
    }

    assert(is_schedule_valid(schedule));
    return schedule;
  }

}
