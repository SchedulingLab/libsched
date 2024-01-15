#ifndef SCHED_SHOP_JOB_LIST_ENGINE_H
#define SCHED_SHOP_JOB_LIST_ENGINE_H

#include <cassert>
#include <algorithm>
#include <optional>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>

#include "JobListInput.h"
#include "JobShopSchedule.h"
#include "JobShopStates.h"

namespace sched::shop {

  struct SCHED_API JobListEngine {
    using Input = JobListInput;

    template<typename Instance>
    std::optional<JobShopSchedule> operator()(const Instance& instance, const JobListInput& input) {
      JobShopStates<Instance> states(instance);
      JobShopSchedule schedule;

      for (auto job : input) {
        const OperationId operation = states.next_operation(job);

        JobShopTask task = {};

        if constexpr (Instance::flexible) {
          const auto available = instance.machines_for_operation(operation);
          assert(!available.empty());

          std::vector<JobShopTask> tasks;

          std::transform(available.begin(), available.end(), std::back_inserter(tasks), [&](MachineId machine) {
            return states.create_task(operation, machine);
          });

          task = *std::min_element(tasks.begin(), tasks.end(), [](const JobShopTask& lhs, const JobShopTask& rhs) {
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
