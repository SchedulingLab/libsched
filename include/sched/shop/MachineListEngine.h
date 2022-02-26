#ifndef SCHED_SWAP_ENGINE_H
#define SCHED_SWAP_ENGINE_H

#include <cassert>
#include <algorithm>
#include <optional>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>

#include "JobShopSchedule.h"
#include "MachineListInput.h"

namespace sched::shop {

  struct SCHED_API MachineListEngine {
    using Input = MachineListInput;

    template<typename Instance>
    std::optional<JobShopSchedule> operator()(const Instance& instance, const MachineListInput& input) {
      struct JobState {
        std::size_t operation = 0;
        Time time = 0;
      };

      std::vector<JobState> jobs(instance.job_count(), JobState{});

      struct MachineState {
        Time time = 0;
        std::size_t index = 0;
      };

      std::vector<MachineState> machines(instance.machine_count(), MachineState{});

      JobShopSchedule schedule;

      for (;;) {
        // try to find a schedulable operation
        bool found = false;
        bool finished = true;

        for (auto machine : sched::machines(instance)) {
          auto& machine_state = machines[to_index(machine)];

          if (machine_state.index == input[to_index(machine)].size()) {
            // there is no more operation to schedule on this machine
            continue;
          }

          finished = false;

          // check if the next operation is schedulable
          auto& op = input[to_index(machine)][machine_state.index];
          auto& job_state = jobs[to_index(op.job)];

          if (op.index == job_state.operation) {
            // we found one
            found = true;

            Time processing_time = instance.processing_time(op, machine);

            JobShopTask task;
            task.operation = op;
            task.machine = machine;
            task.start = std::max({ job_state.time, machine_state.time, instance.release_date(op.job) });
            task.completion = task.start + processing_time;

            machine_state.time = job_state.time = task.completion;

            schedule.append(task);

            ++machine_state.index;
            ++job_state.operation;
            break;
          }
        }

        if (finished) {
          break;
        }

        if (!found) {
          return std::nullopt;
        }
      }

      return schedule;
    }
  };

}

#endif // SCHED_SWAP_ENGINE_H
