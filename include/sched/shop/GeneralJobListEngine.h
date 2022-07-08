#ifndef SCHED_SHOP_GENERAL_JOB_LIST_ENGINE_H
#define SCHED_SHOP_GENERAL_JOB_LIST_ENGINE_H

#include <cassert>
#include <algorithm>
#include <optional>
#include <tuple>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>

#include "JobListInput.h"
#include "GeneralJobShopSchedule.h"

namespace sched::shop {

  struct SCHED_API GeneralJobListEngine {
    using Input = JobListInput;

    template<typename Instance>
    std::optional<GeneralJobShopSchedule> operator()(const Instance& instance, const JobListInput& input) {
      struct JobState {
        MachineId machine = NoMachine;
        std::size_t operation = 0;
        Time time = 0;
      };

      struct TransportationState {
        MachineId machine = NoMachine;
        Time time = 0;
      };

      std::vector<JobState> jobs(instance.job_count(), JobState{});
      std::vector<Time> machines(instance.machine_count(), Time{0});
      std::vector<TransportationState> transportations(instance.transportation_count(), TransportationState{});

      GeneralJobShopSchedule schedule;

      for (auto job : input) {
        JobState& job_state = jobs[to_index(job)];

        assert(job_state.operation < instance.operation_count(job));
        const OperationId op = { job, job_state.operation++ };

        const auto available = instance.machines_for_operation(op);
        assert(!available.empty());

        if (op.index == 0) {
          std::vector<std::tuple<MachineId, JobShopTask>> tasks;

          std::transform(available.begin(), available.end(), std::back_inserter(tasks), [&](MachineId machine) {
            Time processing_time = instance.processing_time(op, machine);

            JobShopTask task;
            task.operation = op;
            task.machine = machine;
            task.start = std::max({ job_state.time, machines[to_index(machine)], instance.release_date(job) });
            task.completion = task.start + processing_time;

            return std::make_tuple(machine, task);
          });

          auto [ machine, task ] = *std::min_element(tasks.begin(), tasks.end(), [](const auto& lhs, const auto& rhs) {
            return std::get<1>(lhs).completion < std::get<1>(rhs).completion;
          });

          job_state.machine = machine;
          machines[to_index(machine)] = job_state.time = task.completion;
          schedule.append(task);
        } else {
          std::vector<std::tuple<MachineId, JobShopTask, TransportationId, TransportationTask, TransportationTask>> tasks;

          for (auto machine : available) {
            for (auto transportation : sched::transportations(instance)) {
              TransportationState& transportation_state = transportations[to_index(transportation)];

              Time empty_time = instance.transportation_time_empty(transportation_state.machine, job_state.machine);

              TransportationTask empty_task;
              empty_task.transportation_resource = transportation;
              empty_task.transportation_kind = TransportationKind::Empty;
              empty_task.origin = transportation_state.machine;
              empty_task.target = job_state.machine;
              empty_task.start = transportation_state.time;
              empty_task.completion = empty_task.start + empty_time;

              Time loaded_time = instance.transportation_time_loaded(job_state.machine, machine);

              TransportationTask loaded_task;
              loaded_task.transportation_resource = transportation;
              loaded_task.transportation_kind = TransportationKind::Loaded;
              loaded_task.origin = job_state.machine;
              loaded_task.target = machine;
              loaded_task.start = std::max({ empty_task.completion, job_state.time });
              loaded_task.completion = loaded_task.start + loaded_time;

              Time processing_time = instance.processing_time(op, machine);

              JobShopTask task;
              task.operation = op;
              task.machine = machine;
              task.start = std::max({ loaded_task.completion, machines[to_index(machine)], instance.release_date(job) });
              task.completion = task.start + processing_time;

              tasks.push_back(std::make_tuple(machine, task, transportation, empty_task, loaded_task));
            }
          }

          auto [ machine, task, transportation, empty_task, loaded_task ] = *std::min_element(tasks.begin(), tasks.end(), [](const auto& lhs, const auto& rhs) {
            return std::get<1>(lhs).completion < std::get<1>(rhs).completion;
          });

          job_state.machine = machine;
          job_state.time = task.completion;

          TransportationState& transportation_state = transportations[to_index(transportation)];
          transportation_state.machine = machine;
          transportation_state.time = loaded_task.completion;

          Time& machine_state = machines[to_index(machine)];
          machine_state = task.completion;

          schedule.append(task);
          schedule.append_transportation_task(empty_task);
          schedule.append_transportation_task(loaded_task);
        }

      }

      return schedule;
    }
  };

}

#endif // SCHED_SHOP_GENERAL_JOB_LIST_ENGINE_H
