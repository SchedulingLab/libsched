// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_INPUT_CONVERSION_H
#define SCHED_SHOP_INPUT_CONVERSION_H

#include <cassert>

#include <tuple>
#include <optional>

#include <sched/meta/input/FloatListInput.h>
#include <sched/shop/input/JobListInput.h>
#include <sched/shop/input/MachineListInput.h>
#include <sched/shop/input/OperationListInput.h>

namespace sched::shop {

  template<typename Instance>
  OperationListInput to_operation_list(const JobListInput& job_list, const Instance& instance)
  {
    OperationListInput operation_list;
    std::vector<std::size_t> job_state(instance.job_count(), 0);

    for (const JobId job : job_list) {
      const std::size_t index = job_state[to_index(job)]++;
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
      auto machine = instance.assigned_machine_for_operation(operation);
      machine_list[to_index(machine)].push_back(operation);
    }

    return machine_list;
  }

  template<typename Instance>
  MachineListInput to_machine_list(const JobListInput& job_list, const Instance& instance)
  {
    auto operation_list = to_operation_list(job_list, instance);
    return to_machine_list(operation_list, instance);
  }

  template<typename Instance>
  OperationListInput to_operation_list(const FloatListInput& float_list, const Instance& instance)
  {
    std::vector<std::tuple<OperationId, double>> operation_order;

    std::size_t index = 0;

    for (const JobId job : jobs(instance)) {
      for (const OperationId operation : operations(instance, job)) {
        assert(index < float_list.size());
        auto element = float_list[index++];
        operation_order.emplace_back(operation, element);
      }
    }

    assert(index == float_list.size());

    std::ranges::sort(operation_order, [](const std::tuple<OperationId, double>& lhs, const std::tuple<OperationId, double>& rhs) {
      return std::get<double>(lhs) < std::get<double>(rhs);
    });

    OperationListInput operation_list;

    std::ranges::transform(operation_order, std::back_inserter(operation_list), [](const std::tuple<OperationId, double> item) {
      return std::get<OperationId>(item);
    });

    return operation_list;
  }

}

#endif // SCHED_SHOP_INPUT_CONVERSION_H
