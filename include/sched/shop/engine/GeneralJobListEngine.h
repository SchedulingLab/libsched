// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_GENERAL_JOB_LIST_ENGINE_H
#define SCHED_SHOP_GENERAL_JOB_LIST_ENGINE_H

#include <cassert>

#include <algorithm>
#include <optional>
#include <vector>

#include <sched/Ids.h>
#include <sched/Instance.h>
#include <sched/shop/input/JobListInput.h>
#include <sched/shop/helper/JobShopTaskComparator.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/shop/helper/JobShopTransportStates.h>
#include <sched/types/ShopInstanceConcepts.h>

namespace sched::shop {

  template<typename Comparator>
  struct GeneralJobListEngine {
    using Input = JobListInput;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
      requires(concepts::ShopTransportInstance<Instance>)
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const JobListInput& input)
    {
      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;
      Comparator comparator;

      for (auto job : input) {
        const OperationId operation = states.next_operation(job);

        if constexpr (Instance::Flexible) {
          const auto available = instance.machines_for_operation(operation);
          assert(!available.empty());

          if (operation.index == 0) {
            std::vector<JobShopTask> tasks;

            std::transform(available.begin(), available.end(), std::back_inserter(tasks), [&](MachineId machine) {
              return states.create_task(operation, machine);
            });

            auto task = *std::ranges::min_element(tasks, comparator);

            states.update_schedule(task, schedule);
          } else {
            std::vector<JobShopTransportTaskPacket> packets;

            for (auto transportation : sched::transportations(instance)) {
              std::transform(available.begin(), available.end(), std::back_inserter(packets), [&](MachineId machine) {
                return states.create_packet(operation, machine, transportation);
              });
            }

            auto packet = *std::ranges::min_element(packets, [comparator](const JobShopTransportTaskPacket& lhs, const JobShopTransportTaskPacket& rhs) {
              return comparator(lhs.task, rhs.task);
            });

            states.update_schedule(packet, schedule);
          }

        } else { // !Flexible
          MachineId machine = instance.assigned_machine_for_operation(operation);

          if (operation.index == 0) {
            JobShopTask task = states.create_task(operation, machine);
            states.update_schedule(task, schedule);
          } else {
            std::vector<JobShopTransportTaskPacket> packets;

            for (auto transportation : sched::transportations(instance)) {
              packets.push_back(states.create_packet(operation, machine, transportation));
            }

            auto packet = *std::ranges::min_element(packets, [comparator](const JobShopTransportTaskPacket& lhs, const JobShopTransportTaskPacket& rhs) {
              return comparator(lhs.task, rhs.task);
            });

            states.update_schedule(packet, schedule);
          }
        }
      }

      return schedule;
    }
  };

  using GeneralJobListEngineEST = GeneralJobListEngine<JobShopTaskEarliestStartingTime>;
  using GeneralJobListEngineLST = GeneralJobListEngine<JobShopTaskLatestStartingTime>;
  using GeneralJobListEngineEFT = GeneralJobListEngine<JobShopTaskEarliestFinishTime>;
  using GeneralJobListEngineLFT = GeneralJobListEngine<JobShopTaskLatestFinishTime>;
  using GeneralJobListEngineSPT = GeneralJobListEngine<JobShopTaskShortestProcessingTime>;
  using GeneralJobListEngineLPT = GeneralJobListEngine<JobShopTaskLargestProcessingTime>;

}

#endif // SCHED_SHOP_GENERAL_JOB_LIST_ENGINE_H
