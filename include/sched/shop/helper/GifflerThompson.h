// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_GIFFLER_THOMPSON_H
#define SCHED_SHOP_GIFFLER_THOMPSON_H

#include <algorithm>
#include <vector>

#include <sched/Ids.h>
#include <sched/shop/schedule/JobShopSchedule.h>
#include <sched/shop/helper/JobShopStates.h>
#include <sched/shop/helper/JobShopTaskComparator.h>

namespace sched::shop {

  template<typename Instance, typename Compare>
  JobShopSchedule giffler_thompson(const Instance& instance, Compare compare) {
    JobShopStates<Instance> states(instance);
    JobShopSchedule schedule;

    std::vector<OperationId> available_operations;
    std::vector<JobShopTask> available_tasks;
    std::vector<JobShopTask> conflicting_tasks;

    for (;;) {
      available_operations.clear();

      for (auto job : jobs(instance)) {
        if (states.has_next_operation(job)) {
          available_operations.push_back(states.next_operation(job));
        }
      }

      if (available_operations.empty()) {
        break;
      }

      available_tasks.clear();

      for (auto operation : available_operations) {
        MachineId machine = instance.assigned_machine_for_operation(operation);
        available_tasks.push_back(states.create_task(operation, machine));
      }

      const JobShopTask earliest_completed_task = *std::ranges::min_element(available_tasks, JobShopTaskEarliestFinishTime());

      conflicting_tasks.clear();

      for (const auto& task : available_tasks) {
        // if (task.operation == earliest_completed_task.operation) {
        //   continue;
        // }

        if (task.machine == earliest_completed_task.machine && task.start < earliest_completed_task.completion) {
          conflicting_tasks.push_back(task);
        }
      }

      if (conflicting_tasks.empty()) {
        states.update_schedule(earliest_completed_task, schedule);
        continue;
      }

      const JobShopTask chosen_task = *std::ranges::min_element(conflicting_tasks, compare);
      states.update_schedule(chosen_task, schedule);
    }

    return schedule;
  }

}

#endif // SCHED_SHOP_GIFFLER_THOMPSON_H
