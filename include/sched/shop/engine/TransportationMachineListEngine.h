// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_TRANSPORTATION_MACHINE_LIST_ENGINE_H
#define SCHED_SHOP_TRANSPORTATION_MACHINE_LIST_ENGINE_H

#include <cassert>

#include <sched/meta/input/FloatListInput.h>
#include <sched/shop/engine/TransportationOperationListEngine.h>
#include <sched/shop/helper/JobShopTransportStates.h>
#include <sched/shop/input/InputConversion.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/meta/input/SplitInput.h>
#include <sched/types/AssignmentTraits.h>
#include <sched/types/EngineTraits.h>

namespace sched::shop {

  template<typename Comparator, typename VehicleAssignment>
  struct TransportationMachineListEngine {
    using Input = SplitInput<FloatListInput, typename VehicleAssignment::Input>;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input)
    {
      auto machine_operations = to_machine_operations(instance);
      auto operation_priority = to_operation_priority(instance, input.input0);
      auto assigned_vehicles = vehicle_assignment(instance, input.input1);

      auto operations_comparator = [&](OperationId lhs, OperationId rhs) {
        return operation_priority[lhs] < operation_priority[rhs];
      };

      for (auto& operations : machine_operations) {
        std::ranges::sort(operations, operations_comparator);
      }

      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;
      Comparator comparator;

      std::size_t vehicle_index = 0;

      while (states.has_pending_operations(machine_operations)) {
        const auto schedulable_operations = states.next_schedulable_operations(machine_operations);

        if (schedulable_operations.empty()) {
          return std::nullopt;
        }

        std::vector<JobShopTask> tasks;
        std::vector<JobShopTransportTaskPacket> packets;

        for (auto [ operation, machine ] : schedulable_operations) {
          if (operation.index == 0) {
            tasks.push_back(states.create_task(operation, machine));
          } else {
            packets.push_back(states.create_packet(operation, machine, assigned_vehicles[vehicle_index++]));
          }
        }

        states.choose_and_update(tasks, packets, schedule, comparator);
        states.update_pending_operations(machine_operations);
      }

      return schedule;
    }

    VehicleAssignment vehicle_assignment;
  };

}

namespace sched {

  template<typename Comparator, typename VehicleAssignment>
  struct EngineTraits<shop::TransportationMachineListEngine<Comparator, VehicleAssignment>> {
    static std::string name()
    {
      using namespace std::literals;
      return "mch_"s + AssignmentTraits<VehicleAssignment>::name() + '_' + Comparator::name();
    }
  };

}

#endif // SCHED_SHOP_TRANSPORTATION_MACHINE_LIST_ENGINE_H
