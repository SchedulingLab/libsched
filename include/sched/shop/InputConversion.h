#ifndef SCHED_SHOP_INPUT_CONVERSION_H
#define SCHED_SHOP_INPUT_CONVERSION_H

#include <cassert>

#include <optional>

#include "JobListInput.h"
#include "MachineListInput.h"
#include "OperationListInput.h"

namespace sched::shop {

  template<typename Instance>
  OperationListInput to_operation_list(const JobListInput& job_list, const Instance& instance)
  {
    OperationListInput operation_list;
    std::vector<std::size_t> job_state(instance.job_count(), 0);

    for (auto& job : job_list) {
      OperationId operation;
      operation.job = job;
      operation.index = job_state[sched::to_index(job)]++;
      operation_list.push_back(operation);
    }

    return operation_list;
  }

  template<typename Instance>
  std::optional<JobListInput> to_job_list(const OperationListInput& operation_list, const Instance& instance)
  {
    JobListInput job_list;
    std::vector<std::size_t> job_state(instance.job_count(), 0);

    for (auto op : operation_list) {
      std::size_t& index = job_state[to_index(op.job)];

      if (op.index != index) {
        return std::nullopt;
      }

      ++index;
      job_list.push_back(op.job);
    }

    return job_list;
  }

  template<typename Instance>
  MachineListInput to_machine_list(const OperationListInput& operation_list, const Instance& instance)
  {
    static_assert(!Instance::flexible, "MachineListInput does not work with flexible instances.");
    MachineListInput machine_list(instance.machine_count());

    for (auto operation : operation_list) {
      auto machine = instance.assigned_machine_for_operation(operation);
      machine_list[sched::to_index(machine)].push_back(operation);
    }

    return machine_list;
  }

  template<typename Instance>
  MachineListInput to_machine_list(const JobListInput& job_list, const Instance& instance)
  {
    auto operation_list = to_operation_list(job_list, instance);
    return to_machine_list(operation_list, instance);
  }

}

#endif // SCHED_SHOP_INPUT_CONVERSION_H
