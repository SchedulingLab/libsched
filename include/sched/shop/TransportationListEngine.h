#ifndef SCHED_SHOP_TRANSPORTATION_LIST_ENGINE_H
#define SCHED_SHOP_TRANSPORTATION_LIST_ENGINE_H

#include <cassert>
#include <algorithm>
#include <optional>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>
#include <sched/common/Instance.h>

#include "TransportationListInput.h"
#include "GeneralJobShopSchedule.h"
#include "GeneralJobShopStates.h"

namespace sched::shop {

  struct SCHED_API TransportationListEngine {
    using Input = TransportationListInput;

    template<typename Instance>
    std::optional<GeneralJobShopSchedule> operator()(const Instance& instance, const TransportationListInput& input) {
      GeneralJobShopStates<Instance> states(instance);
      GeneralJobShopSchedule schedule;

      for (auto job : sched::jobs(instance)) {
        const OperationId operation = states.next_operation(job);

        if constexpr (Instance::flexible) {
          const auto available = instance.machines_for_operation(operation);
          assert(!available.empty());

          std::vector<JobShopTask> tasks;

          std::transform(available.begin(), available.end(), std::back_inserter(tasks), [&](MachineId machine) {
            return states.create_task(operation, machine);
          });

          auto task = *std::min_element(tasks.begin(), tasks.end(), [](const JobShopTask& lhs, const JobShopTask& rhs) {
            return lhs.completion < rhs.completion;
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
          std::vector<GeneralJobShopTaskPacket> packets;

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
          auto packet = *std::min_element(packets.begin(), packets.end(), [](const GeneralJobShopTaskPacket& lhs, const GeneralJobShopTaskPacket& rhs) {
            return lhs.task.completion < rhs.task.completion;
          });

          states.update_schedule(packet, schedule);
        } else { // !flexible
          std::vector<GeneralJobShopTaskPacket> packets;

          for (auto job : sched::jobs(instance)) {
            if (!states.has_next_operation(job)) {
              continue;
            }

            const OperationId operation = states.next_operation(job);
            MachineId machine = instance.assigned_machine_for_operation(operation);
            packets.push_back(states.create_packet(operation, machine, transportation));
          }

          assert(!packets.empty());
          auto packet = *std::min_element(packets.begin(), packets.end(), [](const GeneralJobShopTaskPacket& lhs, const GeneralJobShopTaskPacket& rhs) {
            return lhs.task.completion < rhs.task.completion;
          });

          states.update_schedule(packet, schedule);
        }
      }

      return schedule;
    }

  };

}

#endif // SCHED_SHOP_TRANSPORTATION_LIST_ENGINE_H
