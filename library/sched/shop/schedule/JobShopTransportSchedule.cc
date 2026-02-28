// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/schedule/JobShopTransportSchedule.h>

#include <cassert>

#include <algorithm>
#include <ranges>
#include <tuple>

#include <sched/meta/Instance.h>
#include "sched/Ids.h"

namespace sched::shop {

  namespace {
    bool is_schedule_valid_for_job(const JobShopTransportSchedule& schedule, JobId job)
    {
      auto processing_tasks = schedule.tasks()
        | std::views::filter([job](const JobShopTask& task) { return task.operation.job == job; })
        | std::ranges::to<std::vector<JobShopTask>>();

      auto transportation_tasks = schedule.transportation_tasks()
        | std::views::filter([job](const TransportationTask& task) { return task.job == job; })
        | std::ranges::to<std::vector<TransportationTask>>();

      if (processing_tasks.empty()) {
        return transportation_tasks.empty();
      }

      std::ranges::sort(processing_tasks, [](const JobShopTask& lhs, const JobShopTask& rhs) {
        // also check operations to handle operations with processing time of 0 (like orb07)
        return std::tie(lhs.completion, lhs.operation.index) < std::tie(rhs.completion, rhs.operation.index);
      });

      std::ranges::sort(transportation_tasks, [](const TransportationTask& lhs, const TransportationTask& rhs) {
        return lhs.completion < rhs.completion;
      });

      uint32_t index = 0;
      Time time = 0;
      MachineId machine = NoMachine;

      auto processing_iterator = processing_tasks.begin();
      auto transportation_iterator = transportation_tasks.begin();

      for (;;) {
        // check processing

        if (processing_iterator->start < time) {
          return false;
        }

        time = processing_iterator->completion;

        if (machine != NoMachine) {
          if (processing_iterator->machine != machine) {
            return false;
          }
        } else {
          machine = processing_iterator->machine;
        }

        if (processing_iterator->operation.index != index) {
          return false;
        }

        ++index;

        ++processing_iterator;

        if (processing_iterator == processing_tasks.end()) {
          return transportation_iterator == transportation_tasks.end();
        }

        // check transport

        if (processing_iterator->machine == machine) {
          // the next operation is on the same machine, no need for a transport
          continue;
        }

        if (transportation_iterator == transportation_tasks.end()) {
          // the job need a transport but there is none
          return false;
        }

        if (transportation_iterator->origin != machine) {
          return false;
        }

        machine = transportation_iterator->target;

        if (transportation_iterator->start < time) {
          return false;
        }

        time = transportation_iterator->completion;

        ++transportation_iterator;
      }

      return false;
    }

    bool is_schedule_valid_for_machine(const JobShopTransportSchedule& schedule, MachineId machine)
    {
      auto processing_tasks = schedule.tasks()
        | std::views::filter([machine](const JobShopTask& task) { return task.machine == machine; })
        | std::ranges::to<std::vector<JobShopTask>>();

      std::ranges::sort(processing_tasks, [](const JobShopTask& lhs, const JobShopTask& rhs) {
        // also check operations to handle operations with processing time of 0 (like orb07)
        return lhs.completion < rhs.completion;
      });

      Time time = 0;

      for (const JobShopTask& task :processing_tasks) {

        if (task.start < time) {
          return false;
        }

        time = task.completion;
      }

      return true;
    }

    bool is_schedule_valid_for_vehicle(const JobShopTransportSchedule& schedule, VehicleId vehicle)
    {
      auto transportation_tasks = schedule.transportation_tasks()
        | std::views::filter([vehicle](const TransportationTask& task) { return task.vehicle == vehicle; })
        | std::ranges::to<std::vector<TransportationTask>>();

      std::ranges::sort(transportation_tasks, [](const TransportationTask& lhs, const TransportationTask& rhs) {
        return lhs.completion < rhs.completion;
      });

      MachineId machine = NoMachine;
      Time time = 0;

      for (const TransportationTask& task :transportation_tasks) {
        if (task.start < time) {
          return false;
        }

        time = task.completion;

        if (machine != NoMachine) {
          if (task.origin != machine) {
            return false;
          }
        }

        machine = task.target;
      }

      return true;
    }

  }

  bool is_schedule_valid(const JobShopTransportSchedule& schedule)
  {
    // compute jobs, machines and vehicles

    std::size_t job_count = 0;
    std::size_t machine_count = 0;

    for (const JobShopTask& task : schedule.tasks()) {
      job_count = std::max(job_count, to_index(task.operation.job) + 1);
      machine_count = std::max(machine_count, to_index(task.machine) + 1);
    }

    std::size_t vehicle_count = 0;

    for (const TransportationTask& task : schedule.transportation_tasks()) {
      vehicle_count = std::max(vehicle_count, to_index(task.vehicle) + 1);
    }

    // check jobs

    for (const JobId job : JobRange{ job_count }) {
      if (!is_schedule_valid_for_job(schedule, job)) {
        return false;
      }
    }

    // check machines

    for (const MachineId machine : MachineRange{ machine_count }) {
      if (!is_schedule_valid_for_machine(schedule, machine)) {
        return false;
      }
    }

    // check vehicles

    for (const VehicleId vehicle :VehicleRange{ vehicle_count }) { // NOLINT
      if (!is_schedule_valid_for_vehicle(schedule, vehicle)) {
        return false;
      }
    }

    return true;
  }

}
