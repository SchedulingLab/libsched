// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_FULL_MACHINE_LIST_ENGINE_H
#define SCHED_SHOP_FULL_MACHINE_LIST_ENGINE_H

#include <map>
#include <optional>

#include <sched/shop/helper/JobShopTransportStates.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/meta/input/FloatListInput.h>
#include <sched/meta/input/ThreeInput.h>
#include <sched/types/AssignmentTraits.h>
#include <sched/types/EngineTraits.h>
#include "sched/shop/helper/JobShopStates.h"

namespace sched::shop {

  template<typename MachineAssignment, typename VehicleAssignment>
  struct FullMachineListEngine {
    using Input = ThreeInput<typename MachineAssignment::Input, FloatListInput, typename VehicleAssignment::Input>;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input)
    {
      auto machine_operations = to_machine_operations(instance, input.input0);
      auto operation_priority = to_operation_priority(instance, input.input1);
      auto assigned_vehicles = vehicle_assignment(instance, input.input2);

      auto operations_comparator = [&](OperationId lhs, OperationId rhs) {
        return operation_priority[lhs] < operation_priority[rhs];
      };

      for (auto& operations : machine_operations) {
        std::ranges::sort(operations, operations_comparator);
      }

      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;

      std::size_t vehicle_index = 0;

      while (states.has_waiting_operations(machine_operations)) {

        const auto schedulable_operations = states.next_schedulable_operations(machine_operations);

        if (schedulable_operations.empty()) {
          return std::nullopt;
        }

        const auto [ operation, machine ] = *std::ranges::min_element(schedulable_operations, operations_comparator, [](std::tuple<OperationId, MachineId> element) {
          return std::get<OperationId>(element);
        });

        if (operation.index == 0) {
          const JobShopTask task = states.create_task(operation, machine);
          states.update_schedule(task, schedule);
        } else {
          const JobShopTransportTaskPacket packet = states.create_packet(operation, machine, assigned_vehicles[vehicle_index++]);
          states.update_schedule(packet, schedule);
        }
      }

      return schedule;
    }

    template<typename Instance>
    MachineOperations to_machine_operations(const Instance& instance, const typename MachineAssignment::Input& input)
    {
      auto assigned_machines = machine_assignment(instance, input);

      MachineOperations machine_operations(instance.machine_count());

      for (const JobId job : jobs(instance)) {
        for (const OperationId operation : operations(instance, job)) {
          const MachineId machine = assigned_machines[operation];
          machine_operations[to_index(machine)].push_back(operation);
        }
      }

      return machine_operations;
    }

    template<typename Instance>
    std::map<OperationId, double> to_operation_priority(const Instance& instance, const FloatListInput& input)
    {
      std::map<OperationId, double> operation_priority;
      std::size_t index = 0;

      for (const JobId job : jobs(instance)) {
        for (const OperationId operation : operations(instance, job)) {
          operation_priority.emplace(operation, input[index++]);
        }
      }

      return operation_priority;
    }

    MachineAssignment machine_assignment;
    VehicleAssignment vehicle_assignment;
  };

}

namespace sched {

  template<typename MachineAssignment, typename VehicleAssignment>
  struct EngineTraits<shop::FullMachineListEngine<MachineAssignment, VehicleAssignment>> {
    static std::string name()
    {
      using namespace std::literals;
      return "mch_"s + AssignmentTraits<MachineAssignment>::name() + '_' + AssignmentTraits<VehicleAssignment>::name();
    }
  };

}

#endif // SCHED_SHOP_FULL_MACHINE_LIST_ENGINE_H
