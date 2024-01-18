#ifndef SCHED_SHOP_ASSIGNMENT_ENGINE_H
#define SCHED_SHOP_ASSIGNMENT_ENGINE_H

#include <algorithm>
#include <optional>

#include <sched/common/Api.h>
#include <sched/common/Instance.h>

#include "JobShopSchedule.h"
#include "JobShopStates.h"

namespace sched::shop {

  template<typename Assignment, typename Comparator>
  struct SCHED_API AssignmentEngine {
    using Input = typename Assignment::Input;

    template<typename Instance>
    std::optional<JobShopSchedule> operator()(const Instance& instance, const Input& input) {
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
          auto task = *std::min_element(tasks.begin(), tasks.end(), [comparator](const JobShopTask& lhs, const JobShopTask& rhs) {
            return comparator(lhs, rhs);
          });

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

#endif // SCHED_SHOP_ASSIGNMENT_ENGINE_H
