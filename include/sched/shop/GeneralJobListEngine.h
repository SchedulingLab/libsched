#ifndef SCHED_SHOP_GENERAL_JOB_LIST_ENGINE_H
#define SCHED_SHOP_GENERAL_JOB_LIST_ENGINE_H

#include <cassert>
#include <algorithm>
#include <optional>
#include <tuple>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>

#include "JobListInput.h"
#include "GeneralJobShopSchedule.h"
#include "GeneralJobShopStates.h"

namespace sched::shop {

  struct SCHED_API GeneralJobListEngine {
    using Input = JobListInput;

    template<typename Instance>
    std::optional<GeneralJobShopSchedule> operator()(const Instance& instance, const JobListInput& input) {
      GeneralJobShopStates<Instance> states(instance);
      GeneralJobShopSchedule schedule;

      for (auto job : input) {
        const OperationId operation = states.next_operation(job);

        if constexpr (Instance::flexible) {
          const auto available = instance.machines_for_operation(operation);
          assert(!available.empty());

          if (operation.index == 0) {
            std::vector<JobShopTask> tasks;

            std::transform(available.begin(), available.end(), std::back_inserter(tasks), [&](MachineId machine) {
              return states.create_task(operation, machine);
            });

            auto task = *std::min_element(tasks.begin(), tasks.end(), [](const JobShopTask& lhs, const JobShopTask& rhs) {
              return lhs.completion < rhs.completion;
            });

            states.update_schedule(task, schedule);
          } else {
            std::vector<GeneralJobShopTaskPacket> packets;

            for (auto machine : available) {
              for (auto transportation : sched::transportations(instance)) {
                packets.push_back(states.create_packet(operation, machine, transportation));
              }
            }

            auto packet = *std::min_element(packets.begin(), packets.end(), [](const GeneralJobShopTaskPacket& lhs, const GeneralJobShopTaskPacket& rhs) {
              return lhs.task.completion < rhs.task.completion;
            });

            states.update_schedule(packet, schedule);
          }

        } else { // !flexible
          MachineId machine = instance.assigned_machine_for_operation(operation);

          if (operation.index == 0) {
            JobShopTask task = states.create_task(operation, machine);
            states.update_schedule(task, schedule);
          } else {
            std::vector<GeneralJobShopTaskPacket> packets;

            for (auto transportation : sched::transportations(instance)) {
              packets.push_back(states.create_packet(operation, machine, transportation));
            }

            auto packet = *std::min_element(packets.begin(), packets.end(), [](const GeneralJobShopTaskPacket& lhs, const GeneralJobShopTaskPacket& rhs) {
              return lhs.task.completion < rhs.task.completion;
            });

            states.update_schedule(packet, schedule);
          }
        }
      }

      return schedule;
    }

  };

}

#endif // SCHED_SHOP_GENERAL_JOB_LIST_ENGINE_H
