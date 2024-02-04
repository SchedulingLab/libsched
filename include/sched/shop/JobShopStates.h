#ifndef SCHED_SHOP_JOB_SHOP_STATES_H
#define SCHED_SHOP_JOB_SHOP_STATES_H

#include <vector>

#include <sched/common/Ids.h>

#include "JobShopSchedule.h"

namespace sched::shop {

  template<typename Instance>
  struct JobShopStates {
    JobShopStates(const Instance& instance)
    : instance(instance)
    , jobs(instance.job_count())
    , machines(instance.machine_count())
    {
    }

    bool has_next_operation(JobId job) const
    {
      const JobState& job_state = jobs[to_index(job)];
      return job_state.operation < instance.operation_count(job);
    }

    OperationId next_operation(JobId job) const
    {
      assert(has_next_operation(job));
      const JobState& job_state = jobs[to_index(job)];
      return { job, job_state.operation };
    }

    JobShopTask create_task(OperationId operation, MachineId machine) const
    {
      const JobState& job_state = jobs[to_index(operation.job)];
      const MachineState& machine_state = machines[to_index(machine)];

      JobShopTask task = {};
      task.operation = operation;
      task.machine = machine;
      task.start = std::max({ job_state.time, machine_state.time, instance.release_date(operation.job) });
      task.completion = task.start + instance.processing_time(operation, machine);

      return task;
    }

    void update_schedule(const JobShopTask& task, JobShopSchedule& schedule)
    {
      JobId job = task.operation.job;
      JobState& job_state = jobs[to_index(job)];

      MachineId machine = task.machine;
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

    const Instance& instance;
    std::vector<JobState> jobs;
    std::vector<MachineState> machines;
  };

}

#endif // SCHED_SHOP_JOB_SHOP_STATES_H
