#ifndef SCHED_SHOP_JOB_LIST_ENGINE_H
#define SCHED_SHOP_JOB_LIST_ENGINE_H

#include <cassert>
#include <algorithm>
#include <optional>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>

#include "JobListInput.h"
#include "JobShopSchedule.h"

namespace sched::shop {

  struct SCHED_API JobListEngine {
    using Input = JobListInput;

    template<typename Instance>
    std::optional<JobShopSchedule> operator()(const Instance& instance, const JobListInput& input) {
      struct JobState {
        std::size_t operation = 0;
        Time time = 0;
      };

      std::vector<JobState> jobs(instance.job_count(), JobState{});
      std::vector<Time> machines(instance.machine_count(), Time{0});

      JobShopSchedule schedule;

      for (auto job : input) {
        JobState& job_state = jobs[to_index(job)];

        assert(job_state.operation < instance.operation_count(job));
        const OperationId op = { job, job_state.operation++ };

        JobShopTask task;

        if constexpr (Instance::flexible) {
          const auto available = instance.machines_for_operation(op);
          assert(!available.empty());

          std::vector<JobShopTask> tasks;

          std::transform(available.begin(), available.end(), std::back_inserter(tasks), [&](MachineId machine) {
            Time processing_time = instance.processing_time(op, machine);

            JobShopTask task;
            task.operation = op;
            task.machine = machine;
            task.start = std::max({ job_state.time, machines[to_index(machine)], instance.release_date(job) });
            task.completion = task.start + processing_time;

            return task;
          });

          task = *std::min_element(tasks.begin(), tasks.end(), [](const JobShopTask& lhs, const JobShopTask& rhs) {
            return lhs.completion < rhs.completion;
          });
        } else {
          MachineId machine = instance.assigned_machine_for_operation(op);
          Time processing_time = instance.processing_time(op, machine);

          task.operation = op;
          task.machine = machine;
          task.start = std::max({ job_state.time, machines[to_index(machine)], instance.release_date(job) });
          task.completion = task.start + processing_time;
        }

        machines[to_index(task.machine)] = job_state.time = task.completion;
        schedule.append(task);
      }

      return schedule;
    }
  };

}

#endif // SCHED_SHOP_JOB_LIST_ENGINE_H
