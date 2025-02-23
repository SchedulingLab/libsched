// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_MACHINE_LIST_INPUT_H
#define SCHED_SHOP_MACHINE_LIST_INPUT_H

#include <cassert>

#include <algorithm>
#include <string>
#include <vector>

#include <sched/Ids.h>
#include <sched/meta/Instance.h>
#include <sched/support/Random.h>
#include <sched/types/InputTraits.h>

namespace sched {

  namespace shop {

    using MachineListInput = std::vector<std::vector<OperationId>>;

  }

  template<>
  struct InputTraits<shop::MachineListInput> {
    static std::string name()
    {
      return "mch";
    }

    template<typename Instance>
    static shop::MachineListInput generate_input(const Instance& instance)
    {
      static_assert(!Instance::Flexible, "MachineListInput does not work with flexible instances.");
      shop::MachineListInput input(instance.machine_count());

      for (const JobId job : jobs(instance)) {
        for (const OperationId operation : operations(instance, job)) {
          const MachineId machine = instance.assigned_machine_for_operation(operation);
          input[to_index(machine)].push_back(operation);
        }
      }

      return input;
    }

    template<typename Instance>
    static shop::MachineListInput generate_random(const Instance& instance, Random& random)
    {
      static_assert(!Instance::Flexible, "MachineListInput does not work with flexible instances.");
      shop::MachineListInput input = generate_input(instance);

      for (auto& machine : input) {
        std::ranges::shuffle(machine, random);
      }

      return input;
    }

    template<typename Instance>
    static shop::MachineListInput generate_feasible(const Instance& instance, Random& random)
    {
      static_assert(!Instance::Flexible, "MachineListInput does not work with flexible instances.");
      std::vector<JobId> job_list;

      for (const JobId job : jobs(instance)) {
        const std::size_t operation_count = instance.operation_count(job);
        job_list.insert(job_list.end(), operation_count, job);
      }

      std::ranges::shuffle(job_list, random);

      shop::MachineListInput input(instance.machine_count());

      std::vector<std::size_t> operations(instance.job_count(), 0);

      for (const JobId job : job_list) {
        const std::size_t index = operations[to_index(job)]++;
        const OperationId operation = { .job = job, .index = index };
        const MachineId machine = instance.assigned_machine_for_operation(operation);
        input[to_index(machine)].push_back(operation);
      }

      return input;
    }
  };

}

#endif // SCHED_SHOP_MACHINE_LIST_INPUT_H
