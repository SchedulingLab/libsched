// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_TRANSPORT_STATES_H
#define SCHED_SHOP_JOB_SHOP_TRANSPORT_STATES_H

#include <algorithm>
#include <vector>

#include <sched/Ids.h>
#include <sched/shop/helper/JobShopStates.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>

namespace sched::shop {

  struct JobShopTransportTaskPacket {
    JobShopTask task;
    TransportationTask empty_task;
    TransportationTask loaded_task;
  };

  template<typename Instance>
  struct JobShopTransportStates : JobShopStates<Instance> {
    JobShopTransportStates(const Instance& instance)
    : JobShopStates<Instance>(instance)
    , transportations(instance.transportation_count())
    {
    }

    using States = JobShopStates<Instance>;
    using JobState = typename States::JobState;
    using MachineState = typename States::MachineState;

    using States::update_schedule; // necessary for overloading

    JobShopTransportTaskPacket create_packet(OperationId operation, MachineId machine, TransportationId transportation) const
    {
      const JobState& job_state = States::jobs[to_index(operation.job)];
      const TransportationState& transportation_state = transportations[to_index(transportation)];
      const MachineState& machine_state = States::machines[to_index(machine)];

      JobShopTransportTaskPacket packet = {};

      const Time empty_time = States::instance->transportation_time_empty(transportation_state.machine, job_state.machine);

      packet.empty_task.transportation_resource = transportation;
      packet.empty_task.transportation_kind = TransportationKind::Empty;
      packet.empty_task.origin = transportation_state.machine;
      packet.empty_task.target = job_state.machine;
      packet.empty_task.start = transportation_state.time;
      packet.empty_task.completion = packet.empty_task.start + empty_time;

      Time loaded_time = States::instance->transportation_time_loaded(job_state.machine, machine);

      packet.loaded_task.transportation_resource = transportation;
      packet.loaded_task.transportation_kind = TransportationKind::Loaded;
      packet.loaded_task.origin = job_state.machine;
      packet.loaded_task.target = machine;
      packet.loaded_task.start = std::max({ packet.empty_task.completion, job_state.time });
      packet.loaded_task.completion = packet.loaded_task.start + loaded_time;

      Time processing_time = States::instance->processing_time(operation, machine);

      packet.task.operation = operation;
      packet.task.machine = machine;
      packet.task.start = std::max({ packet.loaded_task.completion, machine_state.time, States::instance->release_date(operation.job) });
      packet.task.completion = packet.task.start + processing_time;

      return packet;
    }

    void update_schedule(const JobShopTransportTaskPacket& packet, JobShopTransportSchedule& schedule)
    {
      const JobId job = packet.task.operation.job;
      JobState& job_state = States::jobs[to_index(job)];

      const MachineId machine = packet.task.machine;
      MachineState& machine_state = States::machines[to_index(machine)];

      const TransportationId transportation = packet.loaded_task.transportation_resource;
      TransportationState& transportation_state = transportations[to_index(transportation)];

      ++job_state.operation;
      job_state.machine = machine;
      job_state.time = packet.task.completion;

      transportation_state.machine = machine;
      transportation_state.time = packet.loaded_task.completion;

      machine_state.time = packet.task.completion;
      ++machine_state.index;

      schedule.append(packet.task);
      schedule.append_transportation_task(packet.empty_task);
      schedule.append_transportation_task(packet.loaded_task);
    }

    template<typename Comparator>
    bool choose_and_update(const std::vector<JobShopTask>& tasks, const std::vector<JobShopTransportTaskPacket>& packets, JobShopTransportSchedule& schedule, Comparator comparator)
    {
      if (!tasks.empty() && !packets.empty()) {
        auto task = *std::ranges::min_element(tasks, comparator);

        auto packet = *std::ranges::min_element(packets, [comparator](const JobShopTransportTaskPacket& lhs, const JobShopTransportTaskPacket& rhs) {
          return comparator(lhs.task, rhs.task);
        });

        if (comparator(task, packet.task)) {
          update_schedule(task, schedule);
        } else {
          update_schedule(packet, schedule);
        }
      } else if (!tasks.empty()) {
        auto task = *std::ranges::min_element(tasks, comparator);

        update_schedule(task, schedule);
      } else if (!packets.empty()) {
        auto packet = *std::ranges::min_element(packets, [comparator](const JobShopTransportTaskPacket& lhs, const JobShopTransportTaskPacket& rhs) {
          return comparator(lhs.task, rhs.task);
        });

        update_schedule(packet, schedule);
      } else {
        return false;
      }

      return true;
    }

    struct TransportationState {
      MachineId machine = NoMachine;
      Time time = 0;
    };

    std::vector<TransportationState> transportations;
  };

}

#endif // SCHED_SHOP_JOB_SHOP_TRANSPORT_STATES_H
