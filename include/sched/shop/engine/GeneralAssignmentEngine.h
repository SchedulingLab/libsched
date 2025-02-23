// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_GENERAL_ASSIGNMENT_ENGINE_H
#define SCHED_SHOP_GENERAL_ASSIGNMENT_ENGINE_H

#include <optional>

#include <sched/Api.h>
#include <sched/meta/Instance.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/shop/helper/JobShopTransportStates.h>
#include <sched/types/AssignmentTraits.h>
#include <sched/types/EngineTraits.h>

namespace sched::shop {

  template<typename MachineAssignment, typename Comparator>
  struct SCHED_API GeneralAssignmentEngine {
    using Input = typename MachineAssignment::Input;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input)
    {
      auto assigned_machines = machine_assignment(instance, input);

      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;
      Comparator comparator;

      for (;;) {
        std::vector<JobShopTask> tasks;
        std::vector<JobShopTransportTaskPacket> packets;

        for (const JobId job : jobs(instance)) {
          if (states.has_next_operation(job)) {
            const OperationId operation = states.next_operation(job);
            assert(assigned_machines.find(operation) != assigned_machines.end());
            const MachineId machine = assigned_machines[operation];

            if (operation.index == 0) {
              tasks.push_back(states.create_task(operation, machine));
            } else {
              for (auto vehicle : vehicles(instance)) {
                packets.push_back(states.create_packet(operation, machine, vehicle));
              }
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

    MachineAssignment machine_assignment;
  };

}

namespace sched {

  template<typename Assignment, typename Comparator>
  struct EngineTraits<shop::GeneralAssignmentEngine<Assignment, Comparator>> {
    static std::string name()
    {
      return AssignmentTraits<Assignment>::name() + '_' + Comparator::name();
    }
  };

}

#endif // SCHED_SHOP_GENERAL_ASSIGNMENT_ENGINE_H
