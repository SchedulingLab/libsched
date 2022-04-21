#ifndef SCHED_OPERATION_LIST_ENGINE_H
#define SCHED_OPERATION_LIST_ENGINE_H

#include <cassert>
#include <algorithm>
#include <optional>
#include <tuple>
#include <vector>

#include <sched/common/Api.h>

#include "JobShopSchedule.h"
#include "OperationListInput.h"

namespace sched::shop {

  struct SCHED_API OperationListEngine {
    using Input = OperationListInput;

    template<typename Instance>
    std::optional<JobShopSchedule> operator()(const Instance& instance, const OperationListInput& input) {
      struct JobState {
        std::size_t operation = 0;
        Time time = 0;
      };

      std::vector<JobState> jobs(instance.job_count());
      std::vector<Time> machines(instance.machine_count(), Time{0});

      JobShopSchedule schedule;

      for (auto op : input) {
        JobState& job_state = jobs[to_index(op.job)];

        if (op.index != job_state.operation) {
          return std::nullopt;
        }

        ++job_state.operation;

        const auto available = instance.machines_for_operation(op);
        assert(!available.empty());

        std::vector<JobShopTask> tasks;

        std::transform(available.begin(), available.end(), std::back_inserter(tasks), [&](MachineId machine) {
          Time processing_time = instance.processing_time(op, machine);

          JobShopTask task;
          task.operation = op;
          task.machine = machine;
          task.start = std::max({ job_state.time, machines[to_index(machine)], instance.release_date(op.job) });
          task.completion = task.start + processing_time;

          return task;
        });

        auto task = *std::min_element(tasks.begin(), tasks.end(), [](const JobShopTask& lhs, const JobShopTask& rhs) {
          return lhs.completion < rhs.completion;
        });

        machines[to_index(task.machine)] = job_state.time = task.completion;
        schedule.append(task);
      }

      return schedule;
    }
  };


}

#endif // SCHED_OPERATION_LIST_ENGINE_H
