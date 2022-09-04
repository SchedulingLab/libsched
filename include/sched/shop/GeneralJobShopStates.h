#ifndef SCHED_SHOP_GENERAL_JOB_SHOP_STATES_H
#define SCHED_SHOP_GENERAL_JOB_SHOP_STATES_H

#include <vector>

#include <sched/common/Ids.h>

#include "GeneralJobShopSchedule.h"

namespace sched::shop {

  struct GeneralJobShopTaskPacket {
    JobShopTask task;
    TransportationTask empty_task;
    TransportationTask loaded_task;
  };

  template<typename Instance>
  struct GeneralJobShopStates {
    GeneralJobShopStates(const Instance& instance)
    : instance(instance)
    , jobs(instance.job_count())
    , machines(instance.machine_count())
    , transportations(instance.transportation_count())
    {
    }

    bool has_next_operation(JobId job) const {
      const JobState& job_state = jobs[to_index(job)];
      return job_state.operation < instance.operation_count(job);
    }

    OperationId next_operation(JobId job) const {
      assert(has_next_operation(job));
      const JobState& job_state = jobs[to_index(job)];
      return { job, job_state.operation };
    }

    JobShopTask create_task(OperationId operation, MachineId machine) const {
      const JobState& job_state = jobs[to_index(operation.job)];
      const MachineState& machine_state = machines[to_index(machine)];

      JobShopTask task;
      task.operation = operation;
      task.machine = machine;
      task.start = std::max({ job_state.time, machine_state.time, instance.release_date(operation.job) });
      task.completion = task.start + instance.processing_time(operation, machine);

      return task;
    }

    void update_schedule(const JobShopTask& task, GeneralJobShopSchedule& schedule) {
      JobId job = task.operation.job;
      JobState& job_state = jobs[to_index(job)];

      MachineId machine = task.machine;
      MachineState& machine_state = machines[to_index(machine)];

      ++job_state.operation;
      job_state.machine = task.machine;
      machine_state.time = job_state.time = task.completion;
      schedule.append(task);
    }

    GeneralJobShopTaskPacket create_packet(OperationId operation, MachineId machine, TransportationId transportation) const {
      const JobState& job_state = jobs[to_index(operation.job)];
      const TransportationState& transportation_state = transportations[to_index(transportation)];
      const MachineState& machine_state = machines[to_index(machine)];

      GeneralJobShopTaskPacket packet;

      Time empty_time = instance.transportation_time_empty(transportation_state.machine, job_state.machine);

      packet.empty_task.transportation_resource = transportation;
      packet.empty_task.transportation_kind = TransportationKind::Empty;
      packet.empty_task.origin = transportation_state.machine;
      packet.empty_task.target = job_state.machine;
      packet.empty_task.start = transportation_state.time;
      packet.empty_task.completion = packet.empty_task.start + empty_time;

      Time loaded_time = instance.transportation_time_loaded(job_state.machine, machine);

      packet.loaded_task.transportation_resource = transportation;
      packet.loaded_task.transportation_kind = TransportationKind::Loaded;
      packet.loaded_task.origin = job_state.machine;
      packet.loaded_task.target = machine;
      packet.loaded_task.start = std::max({ packet.empty_task.completion, job_state.time });
      packet.loaded_task.completion = packet.loaded_task.start + loaded_time;

      Time processing_time = instance.processing_time(operation, machine);

      packet.task.operation = operation;
      packet.task.machine = machine;
      packet.task.start = std::max({ packet.loaded_task.completion, machine_state.time, instance.release_date(operation.job) });
      packet.task.completion = packet.task.start + processing_time;

      return packet;
    }

    void update_schedule(const GeneralJobShopTaskPacket& packet, GeneralJobShopSchedule& schedule) {
      JobId job = packet.task.operation.job;
      JobState& job_state = jobs[to_index(job)];

      MachineId machine = packet.task.machine;
      MachineState& machine_state = machines[to_index(machine)];

      TransportationId transportation = packet.loaded_task.transportation_resource;
      TransportationState& transportation_state = transportations[to_index(transportation)];

      ++job_state.operation;
      job_state.machine = machine;
      job_state.time = packet.task.completion;

      transportation_state.machine = machine;
      transportation_state.time = packet.loaded_task.completion;

      machine_state.time = packet.task.completion;

      schedule.append(packet.task);
      schedule.append_transportation_task(packet.empty_task);
      schedule.append_transportation_task(packet.loaded_task);
    }

    struct JobState {
      MachineId machine = NoMachine;
      std::size_t operation = 0;
      Time time = 0;
    };

    struct MachineState {
      Time time = 0;
    };

    struct TransportationState {
      MachineId machine = NoMachine;
      Time time = 0;
    };

    const Instance& instance;
    std::vector<JobState> jobs;
    std::vector<MachineState> machines;
    std::vector<TransportationState> transportations;
  };

}

#endif // SCHED_SHOP_GENERAL_JOBSHOP_STATES_H
