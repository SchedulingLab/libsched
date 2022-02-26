#ifndef SCHED_SHOP_TIME_ENGINE_H
#define SCHED_SHOP_TIME_ENGINE_H

#include <algorithm>
#include <map>
#include <optional>
#include <queue>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Time.h>
#include <sched/tools/Log.h>

#include "JobShopSchedule.h"
#include "TimeListInput.h"

namespace sched::shop {

  struct SCHED_API TimeListEngine {
    using Input = TimeListInput;

    template<typename Instance>
    std::optional<JobShopSchedule> operator()(const Instance& instance, const TimeListInput& input) {
      std::vector<Time> machines(instance.machine_count(), Time{0});

      struct OperationState {
        OperationId operation;
        Time min_time;
      };

      auto comparator = [](const OperationState& lhs, const OperationState& rhs) {
        return lhs.min_time > rhs.min_time;
      };

      std::priority_queue<OperationState, std::vector<OperationState>, decltype(comparator)> queue(comparator);
      std::map<OperationId, Time> mapping;
      std::size_t index = 0;

      for (auto job : sched::jobs(instance)) {
        auto operations = instance.operation_count(job);

        for (std::size_t i = 0; i < operations; ++i) {
          OperationId operation = { job, i };
          mapping.insert({ operation, input[index++] });
        }

        if (operations > 0) {
          OperationId operation = { job, 0 };
          OperationState operation_state = { operation, mapping[operation] };
          queue.push(operation_state);
        }
      }

      JobShopSchedule schedule;

      while (!queue.empty()) {
        OperationState operation_state = queue.top();
        queue.pop();

        auto available = instance.machines_for_operation(operation_state.operation);

        std::vector<std::tuple<MachineId, JobShopTask>> tasks;

        std::transform(available.begin(), available.end(), std::back_inserter(tasks), [&](MachineId machine) {
          Time processing_time = instance.processing_time(operation_state.operation, machine);

          JobShopTask task;
          task.operation = operation_state.operation;
          task.machine = machine;
          task.start = std::max({ operation_state.min_time, machines[to_index(machine)] });
          task.completion = task.start + processing_time;

          return std::make_tuple(machine, task);
        });

        auto [ machine, task ] = *std::min_element(tasks.begin(), tasks.end(), [](const auto& lhs, const auto& rhs) {
          return std::get<1>(lhs).completion < std::get<1>(rhs).completion;
        });

        machines[to_index(machine)] = task.completion;
        schedule.append(task);

        if (operation_state.operation.index + 1 < instance.operation_count(operation_state.operation.job)) {
          ++operation_state.operation.index;
          operation_state.min_time = task.completion + mapping[operation_state.operation];
          queue.push(operation_state);
        }
      }

      return schedule;
    }

  };

}


#endif // SCHED_SHOP_TIME_ENGINE_H
