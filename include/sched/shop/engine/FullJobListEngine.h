// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_FULL_JOB_LIST_ENGINE_H
#define SCHED_SHOP_FULL_JOB_LIST_ENGINE_H

#include <sched/shop/input/JobListInput.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/shop/helper/JobShopTransportStates.h>
#include <sched/meta/input/ThreeInput.h>
#include <sched/types/AssignmentTraits.h>
#include <sched/types/EngineTraits.h>

namespace sched::shop {

  template<typename MachineAssignment, typename TransportationAssignment>
  struct FullJobListEngine {
    using Input = ThreeInput<typename MachineAssignment::Input, JobListInput, typename TransportationAssignment::Input>;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input)
    {
      auto assignment = machine_assignment(instance, input.input0);
      auto transportation = transportation_assignment(instance, input.input2);

      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;

      std::size_t transportation_index = 0;

      for (auto job : input.input1) {
        assert(transportation_index < transportation.size());
        assert(states.has_next_operation(job));

        const auto operation = states.next_operation(job);
        assert(assignment.find(operation) != assignment.end());
        const auto machine = assignment[operation];

        if (operation.index == 0) {
          JobShopTask task = states.create_task(operation, machine);
          states.update_schedule(task);
        } else {
          const auto vehicle = transportation[transportation_index];
          JobShopTransportTaskPacket packet = states.create_packet(operation, machine, vehicle);
          states.update_schedule(packet);
        }

        ++transportation_index;
      }

      return schedule;
    }

    TransportationAssignment transportation_assignment;
    MachineAssignment machine_assignment;
  };

}

namespace sched {

  template<typename MachineAssignment, typename TransportationAssignment>
  struct EngineTraits<shop::FullJobListEngine<MachineAssignment, TransportationAssignment>> {
    static std::string name()
    {
      using namespace std::literals;
      return "job_"s + AssignmentTraits<MachineAssignment>::name() + '_' + AssignmentTraits<TransportationAssignment>::name();
    }
  };

}

#endif // SCHED_SHOP_FULL_JOB_LIST_ENGINE_H
