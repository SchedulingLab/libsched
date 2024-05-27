#ifndef SCHED_SHOP_TRANSPORTATION_JOB_LIST_ENGINE_H
#define SCHED_SHOP_TRANSPORTATION_JOB_LIST_ENGINE_H

#include <cassert>

#include <algorithm>
#include <optional>
#include <vector>

#include <sched/common/Ids.h>
#include <sched/common/Instance.h>

#include "JobListInput.h"
#include "JobShopTransportSchedule.h"
#include "JobShopTransportStates.h"
#include "SplitInput.h"

namespace sched::shop {

  template<typename Comparator, typename TransportationAssignment>
  struct TransportationJobListEngine {
    using Input = TransportSplitInput<JobListInput, typename TransportationAssignment::Input>;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input)
    {
      auto transportation = transportation_assignment(instance, input.transport);
      std::size_t transportation_index = 0;

      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;
      Comparator comparator;

      for (auto job : input.schedule) {
        assert(transportation_index < transportation.size());
        const OperationId operation = states.next_operation(job);

        if constexpr (Instance::Flexible) {
          const auto available = instance.machines_for_operation(operation);
          assert(!available.empty());

          if (operation.index == 0) {
            std::vector<JobShopTask> tasks;

            std::transform(available.begin(), available.end(), std::back_inserter(tasks), [&](MachineId machine) {
              return states.create_task(operation, machine);
            });

            auto task = *std::min_element(tasks.begin(), tasks.end(), [comparator](const JobShopTask& lhs, const JobShopTask& rhs) {
              return comparator(lhs, rhs);
            });

            states.update_schedule(task, schedule);
          } else {
            std::vector<JobShopTransportTaskPacket> packets;

            std::transform(available.begin(), available.end(), std::back_inserter(packets), [&](MachineId machine) {
              return states.create_packet(operation, machine, transportation[transportation_index]);
            });

            auto packet = *std::min_element(packets.begin(), packets.end(), [comparator](const JobShopTransportTaskPacket& lhs, const JobShopTransportTaskPacket& rhs) {
              return comparator(lhs.task, rhs.task);
            });

            states.update_schedule(packet, schedule);
          }

        } else { // !flexible
          MachineId machine = instance.assigned_machine_for_operation(operation);

          if (operation.index == 0) {
            JobShopTask task = states.create_task(operation, machine);
            states.update_schedule(task, schedule);
          } else {
            JobShopTransportTaskPacket packet = states.create_packet(operation, machine, transportation[transportation_index]);
            states.update_schedule(packet, schedule);
          }
        }

        ++transportation_index;
      }

      return schedule;
    }

    TransportationAssignment transportation_assignment;
  };

}

#endif // SCHED_SHOP_TRANSPORTATION_JOB_LIST_ENGINE_H
