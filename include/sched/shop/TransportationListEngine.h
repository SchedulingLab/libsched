#ifndef SCHED_SHOP_TRANSPORTATION_LIST_ENGINE_H
#define SCHED_SHOP_TRANSPORTATION_LIST_ENGINE_H

#include <cassert>

#include <algorithm>
#include <optional>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>
#include <sched/common/Instance.h>

#include "JobShopTaskComparator.h"
#include "JobShopTransportSchedule.h"
#include "JobShopTransportStates.h"
#include "TransportationListInput.h"

namespace sched::shop {

  template<typename Comparator>
  struct SCHED_API TransportationListEngine {
    using Input = TransportationListInput;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const TransportationListInput& input)
    {
      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;
      Comparator comparator;

      for (auto transportation : input) {
        std::vector<JobShopTask> tasks;
        std::vector<JobShopTransportTaskPacket> packets;

        for (auto job : sched::jobs(instance)) {
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
                return states.create_packet(operation, machine, transportation);
              });
            }
          } else { // !flexible
            MachineId machine = instance.assigned_machine_for_operation(operation);

            if (operation.index == 0) {
              tasks.push_back(states.create_task(operation, machine));
            } else {
              packets.push_back(states.create_packet(operation, machine, transportation));
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

  using TransportationListEngineEST = TransportationListEngine<JobShopTaskEarliestStartingTime>;
  using TransportationListEngineLST = TransportationListEngine<JobShopTaskLatestStartingTime>;
  using TransportationListEngineEFT = TransportationListEngine<JobShopTaskEarliestFinishTime>;
  using TransportationListEngineLFT = TransportationListEngine<JobShopTaskLatestFinishTime>;
  using TransportationListEngineSPT = TransportationListEngine<JobShopTaskShortestProcessingTime>;
  using TransportationListEngineLPT = TransportationListEngine<JobShopTaskLargestProcessingTime>;

}

#endif // SCHED_SHOP_TRANSPORTATION_LIST_ENGINE_H
