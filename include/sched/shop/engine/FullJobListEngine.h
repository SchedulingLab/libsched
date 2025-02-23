// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_FULL_JOB_LIST_ENGINE_H
#define SCHED_SHOP_FULL_JOB_LIST_ENGINE_H

#include <cassert>

#include <optional>

#include <sched/shop/input/JobListInput.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/shop/helper/JobShopTransportStates.h>
#include <sched/meta/input/ThreeInput.h>
#include <sched/types/AssignmentTraits.h>
#include <sched/types/EngineTraits.h>

namespace sched::shop {

  template<typename MachineAssignment, typename VehicleAssignment>
  struct FullJobListEngine {
    using Input = ThreeInput<typename MachineAssignment::Input, JobListInput, typename VehicleAssignment::Input>;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input)
    {
      auto assigned_machines = machine_assignment(instance, input.input0);
      auto assigned_vehicles = vehicle_assignment(instance, input.input2);

      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;

      std::size_t vehicle_index = 0;

      for (const JobId job : input.input1) {
        assert(vehicle_index < assigned_vehicles.size());
        assert(states.has_next_operation(job));

        const OperationId operation = states.next_operation(job);
        assert(assigned_machines.find(operation) != assigned_machines.end());
        const MachineId machine = assigned_machines[operation];

        if (operation.index == 0) {
          JobShopTask task = states.create_task(operation, machine);
          states.update_schedule(task);
        } else {
          const VehicleId vehicle = assigned_vehicles[vehicle_index];
          JobShopTransportTaskPacket packet = states.create_packet(operation, machine, vehicle);
          states.update_schedule(packet);
        }

        ++vehicle_index;
      }

      return schedule;
    }

    VehicleAssignment vehicle_assignment;
    MachineAssignment machine_assignment;
  };

}

namespace sched {

  template<typename MachineAssignment, typename VehicleAssignment>
  struct EngineTraits<shop::FullJobListEngine<MachineAssignment, VehicleAssignment>> {
    static std::string name()
    {
      using namespace std::literals;
      return "job_"s + AssignmentTraits<MachineAssignment>::name() + '_' + AssignmentTraits<VehicleAssignment>::name();
    }
  };

}

#endif // SCHED_SHOP_FULL_JOB_LIST_ENGINE_H
