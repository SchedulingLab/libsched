// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_TRANSPORTATION_LIST_ENGINE_H
#define SCHED_SHOP_TRANSPORTATION_LIST_ENGINE_H

#include <cassert>

#include <algorithm>
#include <optional>
#include <vector>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/meta/Instance.h>
#include <sched/shop/helper/JobShopTaskComparator.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/shop/helper/JobShopTransportStates.h>
#include <sched/shop/input/VehicleListInput.h>
#include <sched/types/EngineTraits.h>

namespace sched::shop {

  template<typename Comparator>
  struct SCHED_API TransportationVehicleListEngine {
    using Input = VehicleListInput;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const VehicleListInput& input)
    {
      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;
      Comparator comparator;

      for (const VehicleId vehicle : input) {
        std::vector<JobShopTask> tasks;
        std::vector<JobShopTransportTaskPacket> packets;

        for (const JobId job : sched::jobs(instance)) {
          if (!states.has_next_operation(job)) {
            continue;
          }

          const OperationId operation = states.next_operation(job);

          if constexpr (Instance::Flexible) {
            const auto available = instance.machines_for_operation(operation);
            assert(!available.empty());

            if (operation.index == 0) {
              std::transform(available.begin(), available.end(), std::back_inserter(tasks), [&](MachineId machine) {
                return states.create_task(operation, machine);
              });
            } else {
              std::transform(available.begin(), available.end(), std::back_inserter(packets), [&](MachineId machine) {
                return states.create_packet(operation, machine, vehicle);
              });
            }
          } else { // !Flexible
            const MachineId machine = instance.assigned_machine_for_operation(operation);

            if (operation.index == 0) {
              tasks.push_back(states.create_task(operation, machine));
            } else {
              packets.push_back(states.create_packet(operation, machine, vehicle));
            }
          }
        }

        if (!states.choose_and_update(tasks, packets, schedule, comparator)) {
          // no more tasks or packets, we have finished
          break;
        }
      }

      return schedule;
    }
  };

  using TransportationVehicleListEngineEST = TransportationVehicleListEngine<JobShopTaskEarliestStartingTime>;
  using TransportationVehicleListEngineLST = TransportationVehicleListEngine<JobShopTaskLatestStartingTime>;
  using TransportationVehicleListEngineEFT = TransportationVehicleListEngine<JobShopTaskEarliestFinishTime>;
  using TransportationVehicleListEngineLFT = TransportationVehicleListEngine<JobShopTaskLatestFinishTime>;
  using TransportationVehicleListEngineSPT = TransportationVehicleListEngine<JobShopTaskShortestProcessingTime>;
  using TransportationVehicleListEngineLPT = TransportationVehicleListEngine<JobShopTaskLargestProcessingTime>;

}

namespace sched {

  template<typename Comparator>
  struct EngineTraits<shop::TransportationVehicleListEngine<Comparator>> {
    static std::string name()
    {
      using namespace std::literals;
      return "trp_"s + Comparator::name();
    }
  };

}

#endif // SCHED_SHOP_TRANSPORTATION_LIST_ENGINE_H
