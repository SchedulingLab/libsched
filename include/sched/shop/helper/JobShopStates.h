// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_STATES_H
#define SCHED_SHOP_JOB_SHOP_STATES_H

#include <cassert>

#include <algorithm>
#include <vector>
#include <tuple>

#include <sched/Ids.h>
#include <sched/support/Range.h>
#include <sched/shop/schedule/JobShopSchedule.h>
#include <sched/shop/helper/MachineOperations.h>

namespace sched::shop {

  template<typename Instance>
  struct JobShopStates {
    JobShopStates(const Instance& instance)
    : instance(&instance)
    , jobs(instance.job_count())
    , machines(instance.machine_count())
    {
    }

    bool has_next_operation(JobId job) const
    {
      const JobState& job_state = jobs[to_index(job)];
      return job_state.operation < instance->operation_count(job);
    }

    OperationId next_operation(JobId job) const
    {
      assert(has_next_operation(job));
      const JobState& job_state = jobs[to_index(job)];
      return { job, job_state.operation };
    }

    std::size_t next_index(MachineId machine) const
    {
      const MachineState& machine_state = machines[to_index(machine)];
      return machine_state.index;
    }

    bool has_pending_operations(const MachineOperations& machine_operations) const
    {
      assert(machine_operations.size() == machines.size());
      return std::ranges::any_of(machines, [&](std::size_t index) { return index < machine_operations[index].size(); }, &MachineState::index);
    }

    void update_pending_operations(const MachineOperations& machine_operations)
    {
      assert(machine_operations.size() == machines.size());

      for (std::size_t machine_index : over(machines)) {
        MachineState& machine_state = machines[machine_index];

        for (;;) {
          if (machine_state.index >= machine_operations[machine_index].size()) {
            break;
          }

          const OperationId machine_operation = machine_operations[machine_index][machine_state.index];

          if (!has_next_operation(machine_operation.job)) {
            // this operation has already been scheduled
            ++machine_state.index;
            continue;
          }

          const OperationId job_operation = next_operation(machine_operation.job);

          if (machine_operation.index < job_operation.index) {
            // this operation has already been scheduled
            ++machine_state.index;
            continue;
          }

          break;
        }
      }
    }

    std::vector<std::tuple<OperationId, MachineId>> next_schedulable_operations(const MachineOperations& machine_operations) const
    {
      assert(machine_operations.size() == machines.size());

      std::vector<std::tuple<OperationId, MachineId>> schedulable_operations;

      for (std::size_t machine_index : over(machines)) {
        const MachineState& machine_state = machines[machine_index];

        // check if the next operation is schedulable

        if (machine_state.index >= machine_operations[machine_index].size()) {
          break;
        }

        const OperationId machine_operation = machine_operations[machine_index][machine_state.index];
        assert(has_next_operation(machine_operation.job));

        const OperationId job_operation = next_operation(machine_operation.job);
        assert(machine_operation.index >= job_operation.index);

        if (machine_operation.index == job_operation.index) {
          // we found one
          schedulable_operations.emplace_back(machine_operation, MachineId{ machine_index });
        }
      }

      return schedulable_operations;
    }

    JobShopTask create_task(OperationId operation, MachineId machine) const
    {
      const JobState& job_state = jobs[to_index(operation.job)];
      const MachineState& machine_state = machines[to_index(machine)];

      JobShopTask task = {};
      task.operation = operation;
      task.machine = machine;
      task.start = std::max({ job_state.time, machine_state.time, instance->release_date(operation.job) });
      task.completion = task.start + instance->processing_time(operation, machine);

      return task;
    }

    void update_schedule(const JobShopTask& task, JobShopSchedule& schedule)
    {
      const JobId job = task.operation.job;
      JobState& job_state = jobs[to_index(job)];

      const MachineId machine = task.machine;
      MachineState& machine_state = machines[to_index(machine)];

      ++job_state.operation;
      job_state.machine = task.machine;
      machine_state.time = job_state.time = task.completion;
      ++machine_state.index;
      schedule.append(task);
    }

    struct JobState {
      MachineId machine = NoMachine;
      std::size_t operation = 0;
      Time time = 0;
    };

    struct MachineState {
      Time time = 0;
      std::size_t index = 0;
    };

    const Instance* instance = nullptr;
    std::vector<JobState> jobs;
    std::vector<MachineState> machines;
  };

}

#endif // SCHED_SHOP_JOB_SHOP_STATES_H
