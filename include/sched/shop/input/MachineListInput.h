// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_MACHINE_LIST_INPUT_H
#define SCHED_SHOP_MACHINE_LIST_INPUT_H

#include <cassert>

#include <algorithm>
#include <string>
#include <vector>

#include <sched/Ids.h>
#include <sched/Instance.h>
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

      for (auto job : sched::jobs(instance)) {
        std::size_t operation_count = instance.operation_count(job);

        for (std::size_t i = 0; i < operation_count; ++i) {
          const OperationId op = { job, i };
          auto machine = instance.assigned_machine_for_operation(op);
          input[sched::to_index(machine)].push_back(op);
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
        std::shuffle(machine.begin(), machine.end(), random);
      }

      return input;
    }

    template<typename Instance>
    static shop::MachineListInput generate_feasible(const Instance& instance, Random& random)
    {
      static_assert(!Instance::Flexible, "MachineListInput does not work with flexible instances.");
      std::vector<JobId> jobs;

      for (auto job : sched::jobs(instance)) {
        auto operations = instance.operation_count(job);

        for (std::size_t i = 0; i < operations; ++i) {
          jobs.push_back(job);
        }
      }

      std::shuffle(jobs.begin(), jobs.end(), random);

      shop::MachineListInput input(instance.machine_count());

      std::vector<std::size_t> operations(instance.job_count(), 0);

      for (auto job : jobs) {
        const std::size_t index = operations[sched::to_index(job)]++;
        OperationId operation = { .job = job, .index = index };
        auto machine = instance.assigned_machine_for_operation(operation);
        input[sched::to_index(machine)].push_back(operation);
      }

      return input;
    }
  };

}

#endif // SCHED_SHOP_MACHINE_LIST_INPUT_H
