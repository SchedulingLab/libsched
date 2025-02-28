// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SWAP_ENGINE_H
#define SCHED_SWAP_ENGINE_H

#include <optional>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/shop/schedule/JobShopSchedule.h>
#include <sched/shop/helper/JobShopStates.h>
#include <sched/shop/input/MachineListInput.h>
#include <sched/types/EngineTraits.h>
#include <sched/types/ShopInstanceConcepts.h>

namespace sched::shop {

  struct SCHED_API MachineListEngine {
    using Input = MachineListInput;
    using Schedule = JobShopSchedule;

    template<typename Instance>
      requires(concepts::ShopInstance<Instance>)
    std::optional<JobShopSchedule> operator()(const Instance& instance, const MachineListInput& input)
    {
      JobShopStates<Instance> states(instance);
      JobShopSchedule schedule;

      for (;;) {
        // try to find a schedulable operation
        bool found = false;
        bool finished = true;

        for (const MachineId machine : machines(instance)) {
          const std::size_t index = states.next_index(machine);

          if (index == input[to_index(machine)].size()) {
            // there is no more operation to schedule on this machine
            continue;
          }

          finished = false;

          // check if the next operation is schedulable

          const OperationId machine_operation = input[to_index(machine)][index];

          if (!states.has_next_operation(machine_operation.job)) {
            continue;
          }

          const OperationId job_operation = states.next_operation(machine_operation.job);

          if (machine_operation.index == job_operation.index) {
            // we found one
            found = true;

            JobShopTask task = states.create_task(machine_operation, machine);
            states.update_schedule(task, schedule);
            break;
          }
        }

        if (finished) {
          break;
        }

        if (!found) {
          return std::nullopt;
        }
      }

      return schedule;
    }
  };

}

namespace sched {

  template<>
  struct EngineTraits<shop::MachineListEngine> {
    static std::string name()
    {
      using namespace std::literals;
      return "mch";
    }
  };

}

#endif // SCHED_SWAP_ENGINE_H
