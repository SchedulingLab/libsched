// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_GENERAL_MACHINE_LIST_ENGINE_H
#define SCHED_SHOP_GENERAL_MACHINE_LIST_ENGINE_H

#include <cassert>

#include <algorithm>
#include <optional>
#include <vector>

#include <sched/Ids.h>
#include <sched/meta/Instance.h>
#include <sched/meta/input/FloatListInput.h>
#include <sched/shop/helper/JobShopTaskComparator.h>
#include <sched/shop/helper/JobShopTransportStates.h>
#include <sched/shop/input/InputConversion.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/types/EngineTraits.h>
#include <sched/types/ShopInstanceConcepts.h>

namespace sched::shop {

  template<typename Comparator>
  struct GeneralMachineListEngine {
    using Input = FloatListInput;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
      requires(concepts::ShopTransportInstance<Instance>)
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const FloatListInput& input)
    {
      auto machine_operations = to_machine_operations(instance);
      auto operation_priority = to_operation_priority(instance, input);

      auto operations_comparator = [&](OperationId lhs, OperationId rhs) {
        return operation_priority[lhs] < operation_priority[rhs];
      };

      for (auto& operations : machine_operations) {
        std::ranges::sort(operations, operations_comparator);
      }

      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;
      Comparator comparator;

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
            for (const VehicleId vehicle : vehicles(instance)) {
              packets.push_back(states.create_packet(operation, machine, vehicle));
            }
          }
        }

        states.choose_and_update(tasks, packets, schedule, comparator);
        states.update_pending_operations(machine_operations);
      }

      return schedule;
    }
  };

  using GeneralMachineListEngineEST = GeneralMachineListEngine<JobShopTaskEarliestStartingTime>;
  using GeneralMachineListEngineLST = GeneralMachineListEngine<JobShopTaskLatestStartingTime>;
  using GeneralMachineListEngineEFT = GeneralMachineListEngine<JobShopTaskEarliestFinishTime>;
  using GeneralMachineListEngineLFT = GeneralMachineListEngine<JobShopTaskLatestFinishTime>;
  using GeneralMachineListEngineSPT = GeneralMachineListEngine<JobShopTaskShortestProcessingTime>;
  using GeneralMachineListEngineLPT = GeneralMachineListEngine<JobShopTaskLargestProcessingTime>;

}

namespace sched {

  template<typename Comparator>
  struct EngineTraits<shop::GeneralMachineListEngine<Comparator>> {
    static std::string name()
    {
      using namespace std::literals;
      return "mch_"s + Comparator::name();
    }
  };

}

#endif // SCHED_SHOP_GENERAL_MACHINE_LIST_ENGINE_H
