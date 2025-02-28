// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_FLOAT_LIST_MACHINE_ASSIGNMENT_H
#define SCHED_SHOP_FLOAT_LIST_MACHINE_ASSIGNMENT_H

#include <cassert>

#include <map>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/meta/Instance.h>
#include <sched/meta/input/FloatListInput.h>
#include <sched/types/AssignmentTraits.h>

namespace sched::shop {

  struct SCHED_API FloatListMachineAssignment {
    using Input = FloatListInput;

    template<typename Instance>
    std::map<OperationId, MachineId> operator()(const Instance& instance, const FloatListInput& input)
    {
      static_assert(Instance::Flexible, "Instance should be flexible.");
      std::map<OperationId, MachineId> assignment;
      std::size_t input_index = 0;

      for (auto job : jobs(instance)) {
        for (auto operation : operations(instance, job)) {
          auto available = instance.machines_for_operation(operation);
          auto machine_count = available.size();
          assert(machine_count > 0);
          assert(input_index < input.size());
          auto machine_index = static_cast<std::size_t>(input[input_index] * static_cast<double>(machine_count));
          assert(machine_index < machine_count);

          [[maybe_unused]] auto [iterator, inserted] = assignment.insert({ operation, available[machine_index] });
          assert(inserted);

          ++input_index;
        }
      }

      return assignment;
    }
  };

}

namespace sched {

  template<>
  struct AssignmentTraits<shop::FloatListMachineAssignment> {
    static std::string name()
    {
      return "mtx";
    }
  };

}

#endif // SCHED_SHOP_FLOAT_LIST_MACHINE_ASSIGNMENT_H
