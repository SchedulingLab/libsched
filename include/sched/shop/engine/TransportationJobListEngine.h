// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_TRANSPORTATION_JOB_LIST_ENGINE_H
#define SCHED_SHOP_TRANSPORTATION_JOB_LIST_ENGINE_H

#include <cassert>

#include <algorithm>
#include <optional>
#include <vector>

#include <sched/Ids.h>
#include <sched/meta/Instance.h>
#include <sched/shop/input/JobListInput.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/shop/helper/JobShopTransportStates.h>
#include <sched/meta/input/SplitInput.h>
#include <sched/types/AssignmentTraits.h>
#include <sched/types/EngineTraits.h>

namespace sched::shop {

  template<typename Comparator, typename TransportationAssignment>
  struct TransportationJobListEngine {
    using Input = SplitInput<JobListInput, typename TransportationAssignment::Input>;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input)
    {
      auto transportation = transportation_assignment(instance, input.input1);
      std::size_t transportation_index = 0;

      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;
      Comparator comparator;

      for (auto job : input.input0) {
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

        } else { // !Flexible
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

namespace sched {

  template<typename Comparator, typename TransportationAssignment>
  struct EngineTraits<shop::TransportationJobListEngine<Comparator, TransportationAssignment>> {
    static std::string name()
    {
      using namespace std::literals;
      return "job_"s + AssignmentTraits<TransportationAssignment>::name() + '_' + Comparator::name();
    }
  };

}

#endif // SCHED_SHOP_TRANSPORTATION_JOB_LIST_ENGINE_H
