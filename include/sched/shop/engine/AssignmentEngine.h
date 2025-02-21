// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_ASSIGNMENT_ENGINE_H
#define SCHED_SHOP_ASSIGNMENT_ENGINE_H

#include <algorithm>
#include <optional>

#include <sched/Api.h>
#include <sched/meta/Instance.h>
#include <sched/shop/schedule/JobShopSchedule.h>
#include <sched/shop/helper/JobShopStates.h>
#include <sched/types/AssignmentTraits.h>
#include <sched/types/EngineTraits.h>

namespace sched::shop {

  template<typename Assignment, typename Comparator>
  struct SCHED_API AssignmentEngine {
    using Input = typename Assignment::Input;
    using Schedule = JobShopSchedule;

    template<typename Instance>
    std::optional<JobShopSchedule> operator()(const Instance& instance, const Input& input)
    {
      auto operations_assignment = assignment(instance, input);

      JobShopStates<Instance> states(instance);
      JobShopSchedule schedule;
      Comparator comparator;

      for (;;) {
        std::vector<JobShopTask> tasks;

        for (auto job : sched::jobs(instance)) {
          if (states.has_next_operation(job)) {
            auto operation = states.next_operation(job);
            assert(operations_assignment.find(operation) != operations_assignment.end());
            tasks.push_back(states.create_task(operation, operations_assignment[operation]));
          }
        }

        if (!tasks.empty()) {
          auto task = *std::ranges::min_element(tasks, comparator);
          states.update_schedule(task, schedule);
        } else {
          break;
        }
      }

      return schedule;
    }

    Assignment assignment;
  };

}

namespace sched {

  template<typename Assignment, typename Comparator>
  struct EngineTraits<shop::AssignmentEngine<Assignment, Comparator>> {
    static std::string name()
    {
      return AssignmentTraits<Assignment>::name() + '_' + Comparator::name();
    }
  };

}

#endif // SCHED_SHOP_ASSIGNMENT_ENGINE_H
