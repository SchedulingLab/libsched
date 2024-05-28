#ifndef SCHED_SHOP_GIFFLER_THOMPSON_H
#define SCHED_SHOP_GIFFLER_THOMPSON_H

#include <algorithm>
#include <vector>

#include <sched/common/Ids.h>

#include "JobShopSchedule.h"
#include "JobShopStates.h"

namespace sched::shop {

  template<typename Instance, typename Compare>
  JobShopSchedule giffler_thompson(const Instance& instance, Compare compare) {
    JobShopStates<Instance> states(instance);
    JobShopSchedule schedule;

    std::vector<JobShopTask> available_tasks;

    auto add_next_operation_if_available = [&states,&instance,&available_tasks](JobId job) {
      if (states.has_next_operation(job)) {
        auto operation = states.next_operation(job);
        MachineId machine = instance.assigned_machine_for_operation(operation);
        available_tasks.push_back(states.create_task(operation, machine));
      }
    };

    for (auto job : jobs(instance)) {
      add_next_operation_if_available(job);
    }

    while (!available_tasks.empty()) {
      const JobShopTask earliest_completed_task = *std::min_element(available_tasks.begin(), available_tasks.end(), [](const JobShopTask& lhs, const JobShopTask& rhs) {
        return lhs.completion < rhs.completion;
      });

      std::vector<JobShopTask> conflicting_tasks;

      for (const auto& task : available_tasks) {
        if (task.operation == earliest_completed_task.operation) {
          continue;
        }

        if (task.machine == earliest_completed_task.machine && task.start < earliest_completed_task.completion) {
          conflicting_tasks.push_back(task);
        }
      }

      const JobShopTask chosen_task = conflicting_tasks.empty() ? earliest_completed_task : *std::min_element(conflicting_tasks.begin(), conflicting_tasks.end(), compare);
      states.update_schedule(chosen_task, schedule);

      auto chosen_iterator = std::find_if(available_tasks.begin(), available_tasks.end(), [&chosen_task](const JobShopTask& task) {
        return task.operation == chosen_task.operation;
      });

      // remove chosen task from the available tasks
      auto last_iterator = --available_tasks.end();
      std::iter_swap(chosen_iterator, last_iterator);
      available_tasks.pop_back();

      add_next_operation_if_available(chosen_task.operation.job);
    }

    return schedule;
  }

}

#endif // SCHED_SHOP_GIFFLER_THOMPSON_H
