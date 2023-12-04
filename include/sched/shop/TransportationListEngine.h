#ifndef SCHED_SHOP_TRANSPORTATION_LIST_ENGINE_H
#define SCHED_SHOP_TRANSPORTATION_LIST_ENGINE_H

#include <cassert>
#include <algorithm>
#include <optional>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>
#include <sched/common/Instance.h>

#include "JobShopTransportSchedule.h"
#include "JobShopTransportStates.h"
#include "JobShopTaskComparator.h"
#include "TransportationListInput.h"

namespace sched::shop {

  template<typename Comparator>
  struct SCHED_API TransportationListEngine {
    using Input = TransportationListInput;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const TransportationListInput& input) {
      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;
      Comparator comparator;

      for (auto job : sched::jobs(instance)) {
        const OperationId operation = states.next_operation(job);

        if constexpr (Instance::flexible) {
          const auto available = instance.machines_for_operation(operation);
          assert(!available.empty());

          std::vector<JobShopTask> tasks;

          std::transform(available.begin(), available.end(), std::back_inserter(tasks), [&](MachineId machine) {
            return states.create_task(operation, machine);
          });

          auto task = *std::min_element(tasks.begin(), tasks.end(), [comparator](const JobShopTask& lhs, const JobShopTask& rhs) {
            return comparator(lhs, rhs);
          });

          states.update_schedule(task, schedule);
        } else { // !flexible
          MachineId machine = instance.assigned_machine_for_operation(operation);
          JobShopTask task = states.create_task(operation, machine);
          states.update_schedule(task, schedule);
        }
      }

      for (auto transportation : input) {
        if constexpr (Instance::flexible) {
          std::vector<JobShopTransportTaskPacket> packets;

          for (auto job : sched::jobs(instance)) {
            if (!states.has_next_operation(job)) {
              continue;
            }

            const OperationId operation = states.next_operation(job);
            const auto available = instance.machines_for_operation(operation);
            assert(!available.empty());

            for (auto machine : available) {
              packets.push_back(states.create_packet(operation, machine, transportation));
            }
          }

          assert(!packets.empty());
          auto packet = *std::min_element(packets.begin(), packets.end(), [comparator](const JobShopTransportTaskPacket& lhs, const JobShopTransportTaskPacket& rhs) {
            return comparator(lhs.task, rhs.task);
          });

          states.update_schedule(packet, schedule);
        } else { // !flexible
          std::vector<JobShopTransportTaskPacket> packets;

          for (auto job : sched::jobs(instance)) {
            if (!states.has_next_operation(job)) {
              continue;
            }

            const OperationId operation = states.next_operation(job);
            MachineId machine = instance.assigned_machine_for_operation(operation);
            packets.push_back(states.create_packet(operation, machine, transportation));
          }

          assert(!packets.empty());
          auto packet = *std::min_element(packets.begin(), packets.end(), [comparator](const JobShopTransportTaskPacket& lhs, const JobShopTransportTaskPacket& rhs) {
            return comparator(lhs.task, rhs.task);
          });

          states.update_schedule(packet, schedule);
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
