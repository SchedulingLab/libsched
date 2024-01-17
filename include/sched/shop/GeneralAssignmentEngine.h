#ifndef SCHED_SHOP_GENERAL_ASSIGNMENT_ENGINE_H
#define SCHED_SHOP_GENERAL_ASSIGNMENT_ENGINE_H

#include <algorithm>
#include <optional>

#include <sched/common/Api.h>
#include <sched/common/Instance.h>

#include "JobShopTransportSchedule.h"
#include "JobShopTransportStates.h"

namespace sched::shop {

  template<typename Assignment, typename Comparator>
  struct SCHED_API GeneralAssignmentEngine {
    using Input = typename Assignment::Input;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input) {
      auto operations_assignment = assignment(instance, input);

      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;
      Comparator comparator;

      for (;;) {
        std::vector<JobShopTask> tasks;
        std::vector<JobShopTransportTaskPacket> packets;

        for (auto job : sched::jobs(instance)) {
          if (states.has_next_operation(job)) {
            auto operation = states.next_operation(job);
            assert(operations_assignment.find(operation) != operations_assignment.end());
            auto machine =  operations_assignment[operation];

            if (operation.index == 0) {
              tasks.push_back(states.create_task(operation, machine));
            } else {
              for (auto transportation : sched::transportations(instance)) {
                packets.push_back(states.create_packet(operation, machine, transportation));
              }
            }
          }
        }

        if (!tasks.empty()) {
          auto task = *std::min_element(tasks.begin(), tasks.end(), [comparator](const JobShopTask& lhs, const JobShopTask& rhs) {
            return comparator(lhs, rhs);
          });

          states.update_schedule(task, schedule);
        } else if (!packets.empty()) {
          auto packet = *std::min_element(packets.begin(), packets.end(), [comparator](const JobShopTransportTaskPacket& lhs, const JobShopTransportTaskPacket& rhs) {
            return comparator(lhs.task, rhs.task);
          });

          states.update_schedule(packet, schedule);
        } else {
          return std::nullopt;
        }
      }

      return schedule;
    }

    Assignment assignment;
  };

}

#endif // SCHED_SHOP_GENERAL_ASSIGNMENT_ENGINE_H
