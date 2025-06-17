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

  template<typename Comparator, typename VehicleAssignment>
  struct TransportationJobListEngine {
    using Input = SplitInput<JobListInput, typename VehicleAssignment::Input>;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input)
    {
      const auto assigned_vehicles = vehicle_assignment(instance, input.input1);
      std::size_t vehicle_index = 0;

      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;

      const JobShopTaskComparatorAdaptor<Comparator, Instance> task_comparator(&instance);
      const JobShopTransportTaskPacketComparatorAdaptor<Comparator, Instance> packet_comparator(&instance);

      for (const JobId job : input.input0) {
        assert(vehicle_index < assigned_vehicles.size());
        const VehicleId vehicle = assigned_vehicles[vehicle_index];
        const OperationId operation = states.next_operation(job);

        if constexpr (Instance::Flexible) {
          const auto available = instance.machines_for_operation(operation);
          assert(!available.empty());

          if (operation.index == 0) {
            std::vector<JobShopTask> tasks;

            std::ranges::transform(available, std::back_inserter(tasks), [&](MachineId machine) {
              return states.create_task(operation, machine);
            });

            auto task = *std::ranges::min_element(tasks, task_comparator);
            states.update_schedule(task, schedule);
          } else {
            std::vector<JobShopTransportTaskPacket> packets;

            std::ranges::transform(available, std::back_inserter(packets), [&](MachineId machine) {
              return states.create_packet(operation, machine, vehicle);
            });

            auto packet = *std::ranges::min_element(packets, packet_comparator);
            states.update_schedule(packet, schedule);
          }

        } else { // !Flexible
          const MachineId machine = instance.assigned_machine_for_operation(operation);

          if (operation.index == 0) {
            const JobShopTask task = states.create_task(operation, machine);
            states.update_schedule(task, schedule);
          } else {
            const JobShopTransportTaskPacket packet = states.create_packet(operation, machine, vehicle);
            states.update_schedule(packet, schedule);
          }
        }

        ++vehicle_index;
      }

      return schedule;
    }

    VehicleAssignment vehicle_assignment;
  };

}

namespace sched {

  template<typename Comparator, typename VehicleAssignment>
  struct EngineTraits<shop::TransportationJobListEngine<Comparator, VehicleAssignment>> {
    static std::string name()
    {
      using namespace std::literals;
      return "job_"s + AssignmentTraits<VehicleAssignment>::name() + '_' + Comparator::name();
    }
  };

}

#endif // SCHED_SHOP_TRANSPORTATION_JOB_LIST_ENGINE_H
