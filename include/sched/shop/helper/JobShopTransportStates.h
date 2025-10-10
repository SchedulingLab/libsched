// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_TRANSPORT_STATES_H
#define SCHED_SHOP_JOB_SHOP_TRANSPORT_STATES_H

#include <cassert>

#include <algorithm>
#include <vector>

#include <sched/Ids.h>
#include <sched/shop/helper/JobShopStates.h>
#include <sched/shop/helper/JobShopTaskComparator.h>
#include <sched/shop/helper/JobShopTransportTaskPacket.h>
#include <sched/shop/helper/JobShopTransportTaskPacketComparator.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>

namespace sched::shop {

  template<typename Instance>
  struct JobShopTransportStates : JobShopStates<Instance> {
    JobShopTransportStates(const Instance& instance)
    : JobShopStates<Instance>(instance)
    , vehicles(instance.vehicle_count())
    {
    }

    using States = JobShopStates<Instance>;
    using JobState = typename States::JobState;
    using MachineState = typename States::MachineState;

    using States::update_schedule; // necessary for overloading

    JobShopTransportTaskPacket create_packet(OperationId operation, MachineId machine, VehicleId vehicle) const
    {
      const JobState& job_state = States::jobs[to_index(operation.job)];
      const VehicleState& vehicle_state = vehicles[to_index(vehicle)];
      const MachineState& machine_state = States::machines[to_index(machine)];

      JobShopTransportTaskPacket packet = {};

      const Time empty_time = States::instance->transportation_time_empty(vehicle_state.machine, job_state.machine);

      packet.empty_task.vehicle = vehicle;
      packet.empty_task.transportation_kind = TransportationKind::Empty;
      packet.empty_task.origin = vehicle_state.machine;
      packet.empty_task.target = job_state.machine;
      packet.empty_task.job = NoJob;
      packet.empty_task.start = vehicle_state.time;
      packet.empty_task.completion = packet.empty_task.start + empty_time;

      const Time loaded_time = States::instance->transportation_time_loaded(job_state.machine, machine);

      packet.loaded_task.vehicle = vehicle;
      packet.loaded_task.transportation_kind = TransportationKind::Loaded;
      packet.loaded_task.origin = job_state.machine;
      packet.loaded_task.target = machine;
      packet.loaded_task.job = operation.job;
      packet.loaded_task.start = std::max({ packet.empty_task.completion, job_state.time });
      packet.loaded_task.completion = packet.loaded_task.start + loaded_time;

      const Time processing_time = States::instance->processing_time(operation, machine);

      packet.task.operation = operation;
      packet.task.machine = machine;
      packet.task.start = std::max({ packet.loaded_task.completion, machine_state.time, States::instance->release_date(operation.job) });
      packet.task.completion = packet.task.start + processing_time;

      return packet;
    }

    void update_schedule(const JobShopTransportTaskPacket& packet, JobShopTransportSchedule& schedule)
    {
      const JobId job = packet.task.operation.job;
      assert(to_index(job) < States::jobs.size());
      JobState& job_state = States::jobs[to_index(job)];

      const MachineId machine = packet.task.machine;
      assert(to_index(machine) < States::machines.size());
      MachineState& machine_state = States::machines[to_index(machine)];

      const VehicleId vehicle = packet.loaded_task.vehicle;
      assert(to_index(vehicle) < vehicles.size());
      VehicleState& vehicle_state = vehicles[to_index(vehicle)];

      ++job_state.operation;
      job_state.machine = machine;
      job_state.time = packet.task.completion;

      vehicle_state.machine = machine;
      vehicle_state.time = packet.loaded_task.completion;

      machine_state.time = packet.task.completion;
      ++machine_state.index;

      schedule.append(packet.task);
      schedule.append_transportation_task(packet.empty_task);
      schedule.append_transportation_task(packet.loaded_task);
    }

    template<typename Comparator>
    bool choose_and_update(const std::vector<JobShopTask>& tasks, const std::vector<JobShopTransportTaskPacket>& packets, JobShopTransportSchedule& schedule, Comparator comparator)
    {
      const JobShopTaskComparatorAdaptor<Comparator, Instance> task_comparator(States::instance);
      const JobShopTransportTaskPacketComparatorAdaptor<Comparator, Instance> packet_comparator(States::instance);

      if (!tasks.empty() && !packets.empty()) {
        auto task = *std::ranges::min_element(tasks, task_comparator);
        auto packet = *std::ranges::min_element(packets, packet_comparator);

        if (comparator(task, packet.task, *States::instance)) {
          update_schedule(task, schedule);
        } else {
          update_schedule(packet, schedule);
        }
      } else if (!tasks.empty()) {
        auto task = *std::ranges::min_element(tasks, task_comparator);
        update_schedule(task, schedule);
      } else if (!packets.empty()) {
        auto packet = *std::ranges::min_element(packets, packet_comparator);
        update_schedule(packet, schedule);
      } else {
        return false;
      }

      return true;
    }

    struct VehicleState {
      MachineId machine = NoMachine;
      Time time = 0;
    };

    std::vector<VehicleState> vehicles;
  };

}

#endif // SCHED_SHOP_JOB_SHOP_TRANSPORT_STATES_H
