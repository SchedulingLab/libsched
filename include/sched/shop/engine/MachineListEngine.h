// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_MACHINE_LIST_ENGINE_H
#define SCHED_SHOP_MACHINE_LIST_ENGINE_H

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

      while (states.has_pending_operations(input)) {
        const auto schedulable_operations = states.next_schedulable_operations(input);

        if (schedulable_operations.empty()) {
          return std::nullopt;
        }

        for (const auto [ operation, machine ] : schedulable_operations) {
          const JobShopTask task = states.create_task(operation, machine);
          states.update_schedule(task, schedule);
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

#endif // SCHED_SHOP_MACHINE_LIST_ENGINE_H
