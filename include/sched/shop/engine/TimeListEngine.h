#ifndef SCHED_SHOP_TIME_ENGINE_H
#define SCHED_SHOP_TIME_ENGINE_H

#include <cassert>

#include <algorithm>
#include <map>
#include <optional>
#include <queue>
#include <vector>

#include <sched/Api.h>
#include <sched/Time.h>
#include <sched/shop/schedule/JobShopSchedule.h>
#include <sched/shop/input/TimeListInput.h>

namespace sched::shop {

  struct SCHED_API TimeListEngine {
    using Input = TimeListInput;
    using Schedule = JobShopSchedule;

    template<typename Instance>
    std::optional<JobShopSchedule> operator()(const Instance& instance, const TimeListInput& input)
    {
      std::vector<Time> machines(instance.machine_count(), Time{ 0 });

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
          const OperationId operation = { .job = job, .index = i };
          mapping.insert({ operation, input[index++] });
        }

        OperationId operation = { job, 0 };
        OperationState operation_state = { operation, mapping[operation] };
        queue.push(operation_state);
      }

      assert(mapping.size() == input.size());

      JobShopSchedule schedule;

      while (!queue.empty()) {
        OperationState operation_state = queue.top();
        queue.pop();

        const OperationId op = operation_state.operation;
        JobShopTask task = {};

        if constexpr (Instance::Flexible) {
          auto available = instance.machines_for_operation(op);
          assert(!available.empty());

          std::vector<JobShopTask> tasks;

          std::transform(available.begin(), available.end(), std::back_inserter(tasks), [&](MachineId machine) {
            Time processing_time = instance.processing_time(op, machine);

            JobShopTask task = {};
            task.operation = op;
            task.machine = machine;
            task.start = std::max({ operation_state.min_time, machines[to_index(machine)] });
            task.completion = task.start + processing_time;

            return task;
          });

          task = *std::min_element(tasks.begin(), tasks.end(), [](const auto& lhs, const auto& rhs) {
            return lhs.completion < rhs.completion;
          });
        } else {
          MachineId machine = instance.assigned_machine_for_operation(op);
          Time processing_time = instance.processing_time(op, machine);

          task.operation = op;
          task.machine = machine;
          task.start = std::max({ operation_state.min_time, machines[to_index(machine)] });
          task.completion = task.start + processing_time;
        }

        machines[to_index(task.machine)] = task.completion;
        schedule.append(task);

        if (op.index + 1 < instance.operation_count(op.job)) {
          ++operation_state.operation.index;
          operation_state.min_time = task.completion + mapping[operation_state.operation];
          queue.push(operation_state);
        }
      }

      assert(schedule.task_count() == input.size());
      return make_schedule_active(schedule);
    }
  };

}

#endif // SCHED_SHOP_TIME_ENGINE_H
