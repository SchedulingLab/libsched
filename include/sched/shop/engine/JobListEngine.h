// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_LIST_ENGINE_H
#define SCHED_SHOP_JOB_LIST_ENGINE_H

#include <cassert>

#include <algorithm>
#include <optional>
#include <vector>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/shop/input/JobListInput.h>
#include <sched/shop/schedule/JobShopSchedule.h>
#include <sched/shop/helper/JobShopStates.h>
#include <sched/types/ShopInstanceConcepts.h>

namespace sched::shop {

  struct SCHED_API JobListEngine {
    using Input = JobListInput;
    using Schedule = JobShopSchedule;

    template<typename Instance>
      requires(concepts::ShopInstance<Instance>)
    std::optional<JobShopSchedule> operator()(const Instance& instance, const JobListInput& input)
    {
      JobShopStates<Instance> states(instance);
      JobShopSchedule schedule;

      for (auto job : input) {
        const OperationId operation = states.next_operation(job);

        JobShopTask task = {};

        if constexpr (Instance::Flexible) {
          const auto available = instance.machines_for_operation(operation);
          assert(!available.empty());

          std::vector<JobShopTask> tasks;

          std::transform(available.begin(), available.end(), std::back_inserter(tasks), [&](MachineId machine) {
            return states.create_task(operation, machine);
          });

          task = *std::ranges::min_element(tasks, [](const JobShopTask& lhs, const JobShopTask& rhs) {
            return lhs.completion < rhs.completion;
          });
        } else {
          MachineId machine = instance.assigned_machine_for_operation(operation);
          task = states.create_task(operation, machine);
        }

        states.update_schedule(task, schedule);
      }

      return schedule;
    }
  };

}

#endif // SCHED_SHOP_JOB_LIST_ENGINE_H
