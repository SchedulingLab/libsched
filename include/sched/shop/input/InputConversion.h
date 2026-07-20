// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_INPUT_CONVERSION_H
#define SCHED_SHOP_INPUT_CONVERSION_H

#include <cassert>
#include <cstdint>

#include <optional>

#include <sched/shop/helper/MachineOperations.h>
#include <sched/shop/helper/OperationPriority.h>
#include <sched/meta/input/FloatListInput.h>
#include <sched/shop/input/JobListInput.h>
#include <sched/shop/input/MachineListInput.h>
#include <sched/shop/input/OperationListInput.h>

namespace sched::shop {

  namespace details {

    template<typename Instance>
    std::vector<double> extract_float_list_for_job(const Instance& instance, const FloatListInput& input, std::size_t& index, JobId job)
    {
      const std::size_t count = instance.operation_count(job);
      std::vector<double> float_list(count);

      for (std::size_t i = 0; i < count; ++i) {
        assert(index < input.size());
        float_list[i] = input[index++];
      }

      // preserve the operation order to make the input feasible
      std::ranges::sort(float_list);

      return float_list;
    }


  }

  template<typename Instance>
  OperationListInput to_operation_list(const JobListInput& job_list, const Instance& instance)
  {
    OperationListInput operation_list;
    std::vector<uint32_t> job_state(instance.job_count(), 0);

    for (const JobId job : job_list) {
      const uint32_t index = job_state[to_index(job)]++;
      const OperationId operation = { .job = job, .index = index };
      operation_list.push_back(operation);
    }

    return operation_list;
  }

  template<typename Instance>
  std::optional<JobListInput> to_job_list(const OperationListInput& operation_list, const Instance& instance)
  {
    JobListInput job_list;
    std::vector<std::size_t> job_state(instance.job_count(), 0);

    for (const OperationId operation : operation_list) {
      std::size_t& index = job_state[to_index(operation.job)];

      if (operation.index != index) {
        return std::nullopt;
      }

      ++index;
      job_list.push_back(operation.job);
    }

    return job_list;
  }

  template<typename Instance>
  MachineListInput to_machine_list(const OperationListInput& operation_list, const Instance& instance)
  {
    static_assert(!Instance::Flexible, "MachineListInput does not work with flexible instances.");
    MachineListInput machine_list(instance.machine_count());

    for (const OperationId operation : operation_list) {
      const MachineId machine = instance.assigned_machine_for_operation(operation);
      machine_list[to_index(machine)].push_back(operation);
    }

    return machine_list;
  }

  template<typename Instance>
  MachineListInput to_machine_list(const JobListInput& job_list, const Instance& instance)
  {
    const OperationListInput operation_list = to_operation_list(job_list, instance);
    return to_machine_list(operation_list, instance);
  }

  template<typename Instance>
  OperationListInput to_operation_list(const FloatListInput& float_list, const Instance& instance)
  {
    struct FloatOperation {
      OperationId operation;
      double value;
    };

    std::vector<FloatOperation> operation_order;

    std::size_t index = 0;

    for (const JobId job : jobs(instance)) {
      const std::vector<double> float_list_for_job = details::extract_float_list_for_job(instance, float_list, index, job);

      for (const OperationId operation : operations(instance, job)) {
        assert(operation.index < float_list_for_job.size());
        const double value = float_list_for_job[operation.index];
        operation_order.emplace_back(operation, value);
      }
    }

    assert(index == float_list.size());
    std::ranges::sort(operation_order, {}, &FloatOperation::value);

    OperationListInput operation_list;
    std::ranges::transform(operation_order, std::back_inserter(operation_list), &FloatOperation::operation);

    return operation_list;
  }

  template<typename Instance>
  MachineOperations to_machine_operations(const Instance& instance)
  {
    MachineOperations machine_operations(instance.device_count());

    for (const JobId job : jobs(instance)) {
      for (const OperationId operation : operations(instance, job)) {
        if constexpr (Instance::Flexible) {
          const std::vector<MachineId> available = instance.machines_for_operation(operation);

          for (const MachineId machine : available) {
            machine_operations[to_index(machine)].push_back(operation);
          }
        } else {
          const MachineId machine = instance.assigned_machine_for_operation(operation);
          machine_operations[to_index(machine)].push_back(operation);
        }
      }
    }

    return machine_operations;
  }

  template<typename Instance>
  OperationPriority to_operation_priority(const Instance& instance, const FloatListInput& float_list)
  {
    OperationPriority operation_priority;
    std::size_t index = 0;

    for (const JobId job : jobs(instance)) {
      const std::vector<double> float_list_for_job = details::extract_float_list_for_job(instance, float_list, index, job);

      for (const OperationId operation : operations(instance, job)) {
        assert(operation.index < float_list_for_job.size());
        const double value = float_list_for_job[operation.index];
        operation_priority.emplace(operation, value);
      }
    }

    return operation_priority;
  }

}

#endif // SCHED_SHOP_INPUT_CONVERSION_H
