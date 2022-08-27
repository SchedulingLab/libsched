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

      auto add_packets_for_machine = [&](JobId job, OperationId op, MachineId machine, std::vector<TaskPacket>& packets) {
        JobState& job_state = jobs[to_index(job)];

        for (auto transportation : sched::transportations(instance)) {
          TaskPacket packet;

          TransportationState& transportation_state = transportations[to_index(transportation)];

          Time empty_time = instance.transportation_time_empty(transportation_state.machine, job_state.machine);

          packet.empty_task.transportation_resource = transportation;
          packet.empty_task.transportation_kind = TransportationKind::Empty;
          packet.empty_task.origin = transportation_state.machine;
          packet.empty_task.target = job_state.machine;
          packet.empty_task.start = transportation_state.time;
          packet.empty_task.completion = packet.empty_task.start + empty_time;

          Time loaded_time = instance.transportation_time_loaded(job_state.machine, machine);

          packet.loaded_task.transportation_resource = transportation;
          packet.loaded_task.transportation_kind = TransportationKind::Loaded;
          packet.loaded_task.origin = job_state.machine;
          packet.loaded_task.target = machine;
          packet.loaded_task.start = std::max({ packet.empty_task.completion, job_state.time });
          packet.loaded_task.completion = packet.loaded_task.start + loaded_time;

          Time processing_time = instance.processing_time(op, machine);

          packet.task.operation = op;
          packet.task.machine = machine;
          packet.task.start = std::max({ packet.loaded_task.completion, machines[to_index(machine)], instance.release_date(job) });
          packet.task.completion = packet.task.start + processing_time;

          packets.push_back(std::move(packet));
        }
      };

      GeneralJobShopSchedule schedule;

      auto update_with_packet = [&](JobId job, const TaskPacket& packet) {
        JobState& job_state = jobs[to_index(job)];

        MachineId machine = packet.task.machine;
        TransportationId transportation = packet.loaded_task.transportation_resource;

        job_state.machine = machine;
        job_state.time = packet.task.completion;

        TransportationState& transportation_state = transportations[to_index(transportation)];
        transportation_state.machine = machine;
        transportation_state.time = packet.loaded_task.completion;

        Time& machine_state = machines[to_index(machine)];
        machine_state = packet.task.completion;

        schedule.append(packet.task);
        schedule.append_transportation_task(packet.empty_task);
        schedule.append_transportation_task(packet.loaded_task);
      };

      for (auto job : input) {
        JobState& job_state = jobs[to_index(job)];

        assert(job_state.operation < instance.operation_count(job));
        const OperationId op = { job, job_state.operation++ };

        if constexpr (Instance::flexible) {
          const auto available = instance.machines_for_operation(op);
          assert(!available.empty());

          if (op.index == 0) {
            std::vector<JobShopTask> tasks;

            std::transform(available.begin(), available.end(), std::back_inserter(tasks), [&](MachineId machine) {
              Time processing_time = instance.processing_time(op, machine);

              JobShopTask task;
              task.operation = op;
              task.machine = machine;
              task.start = std::max({ job_state.time, machines[to_index(machine)], instance.release_date(job) });
              task.completion = task.start + processing_time;

              return task;
            });

            auto task = *std::min_element(tasks.begin(), tasks.end(), [](const JobShopTask& lhs, const JobShopTask& rhs) {
              return lhs.completion < rhs.completion;
            });

            job_state.machine = task.machine;
            machines[to_index(task.machine)] = job_state.time = task.completion;
            schedule.append(task);
          } else {
            std::vector<TaskPacket> packets;

            for (auto machine : available) {
              add_packets_for_machine(job, op, machine, packets);
            }

            auto packet = *std::min_element(packets.begin(), packets.end(), [](const TaskPacket& lhs, const TaskPacket& rhs) {
              return lhs.task.completion < rhs.task.completion;
            });

            update_with_packet(job, packet);
          }

        } else { // !flexible
          MachineId machine = instance.assigned_machine_for_operation(op);
          Time processing_time = instance.processing_time(op, machine);

          if (op.index == 0) {
            JobShopTask task;
            task.operation = op;
            task.machine = machine;
            task.start = std::max({ job_state.time, machines[to_index(machine)], instance.release_date(job) });
            task.completion = task.start + processing_time;

            job_state.machine = task.machine;
            machines[to_index(task.machine)] = job_state.time = task.completion;
            schedule.append(task);
          } else {
            std::vector<TaskPacket> packets;
            add_packets_for_machine(job, op, machine, packets);

            auto packet = *std::min_element(packets.begin(), packets.end(), [](const TaskPacket& lhs, const TaskPacket& rhs) {
              return lhs.task.completion < rhs.task.completion;
            });

            update_with_packet(job, packet);
          }
        }
      }

      return schedule;
    }

    struct TaskPacket {
      JobShopTask task;
      TransportationTask empty_task;
      TransportationTask loaded_task;
    };
  };

}

#endif // SCHED_SHOP_GENERAL_JOB_LIST_ENGINE_H
