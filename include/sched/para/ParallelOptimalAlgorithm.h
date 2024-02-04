#ifndef SCHED_PARA_PARALLEL_OPTIMAL_ALGORITHM_H
#define SCHED_PARA_PARALLEL_OPTIMAL_ALGORITHM_H

#include <cassert>

#include <vector>

#include <lqp/GlpkSolver.h>
#include <lqp/Problem.h>

#include <sched/common/Api.h>
#include <sched/common/Instance.h>
#include <sched/common/MakespanCriterion.h>

#include "ParallelJob.h"
#include "ParallelSchedule.h"

namespace sched::para {

  struct SCHED_API ParallelOptimalAlgorithm {

    template<typename Instance>
    ParallelSchedule operator()(const Instance& instance)
    {
      std::vector<ParallelJob> jobs;

      for (auto job : sched::jobs(instance)) {
        jobs.push_back({ job, instance.processing_time(job, AnyMachine) });
      }

      [[maybe_unused]] const std::size_t job_count = instance.job_count();
      assert(job_count == jobs.size());
      std::size_t machine_count = instance.machine_count();

      lqp::Problem problem;

      /* variables:
       * - x_i_j: binary variable, 1 if job i is scheduled on machine j, 0 otherwise
       * - c_max: makespan of the schedule
       */

      auto x_i_j = [machine_count](JobId job, std::size_t machine) {
        return lqp::VariableId{ to_index(job) * machine_count + machine };
      };

      for (auto& job : jobs) {
        for (std::size_t machine = 0; machine < machine_count; ++machine) {
          [[maybe_unused]] auto variable = problem.add_variable(lqp::VariableCategory::Binary, "x_" + std::to_string(to_index(job.id)) + "_" + std::to_string(machine));
          assert(x_i_j(job.id, machine) == variable);
        }
      }

      auto c_max = problem.add_variable(lqp::VariableCategory::Integer, lqp::lower_bound(0.0), "c_max");

      /* constraints:
       * - sum_{j} x_i_j = 1 (job i is scheduled on one and only one machine)
       * - sum_{i} x_i_j * p_i <= c_max (c_max is greater than the sum of the processing times of all tasks present on machine j)
       */

      for (auto& job : jobs) {
        lqp::QExpr sum;

        for (std::size_t machine = 0; machine < machine_count; ++machine) {
          sum += x_i_j(job.id, machine);
        }

        problem.add_constraint(sum == 1.0, "job_" + std::to_string(to_index(job.id)));
      }

      for (std::size_t machine = 0; machine < machine_count; ++machine) {
        lqp::QExpr sum;

        for (auto& job : jobs) {
          sum += job.processing_time * x_i_j(job.id, machine);
        }

        problem.add_constraint(sum <= c_max, "machine_" + std::to_string(machine));
      }

      problem.set_objective(lqp::Sense::Minimize, c_max, "c_max");

      lqp::GlpkSolver solver;

      lqp::SolverConfig config;
      config.use_mip = true;
      config.presolve = true;
      config.verbose = false;

      auto solution = solver.solve(problem, config);

      ParallelSchedule schedule;
      std::vector<Time> machines(machine_count, 0);

      for (auto& job : jobs) {
        [[maybe_unused]] bool unique = false;

        for (std::size_t machine = 0; machine < machine_count; ++machine) {
          double value = solution.value(x_i_j(job.id, machine));

          if (value == 1.0) {
            assert(!unique);
            unique = true;

            ParallelTask task = {};
            task.job = job.id;
            task.machine = MachineId{ machine };
            task.start = machines[machine];
            task.completion = task.start + job.processing_time;
            schedule.append(task);

            machines[machine] += job.processing_time;
          }
        }
      }

      return schedule;
    }
  };

}

#endif // SCHED_PARA_PARALLEL_OPTIMAL_ALGORITHM_H
