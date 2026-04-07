// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/schedule/JobShopSchedule.h>

#include <cassert>

#include <algorithm>
#include <tuple>

#include <sched/IdsFmt.h>
#include <sched/meta/Instance.h>
#include <sched/support/Log.h>
#include <sched/shop/instance/FlexibleJobShopInstance.h>
#include <sched/shop/instance/JobShopInstance.h>

namespace sched::shop {

  namespace {

    bool is_schedule_valid_for_job(const JobShopSchedule& schedule, JobId job)
    {
      auto processing_tasks = schedule.tasks()
        | std::views::filter([job](const JobShopTask& task) { return task.operation.job == job; })
        | std::ranges::to<std::vector<JobShopTask>>();

      if (processing_tasks.empty()) {
        return false;
      }

      std::ranges::stable_sort(processing_tasks, [](const JobShopTask& lhs, const JobShopTask& rhs) {
        // also check operations to handle operations with processing time of 0 (like orb07)
        return std::tie(lhs.start, lhs.completion, lhs.operation.index) < std::tie(rhs.start, rhs.completion, rhs.operation.index);
      });

      uint32_t index = 0;
      Time time = 0;
      MachineId machine = NoMachine;

      for (const JobShopTask& task : processing_tasks) {
        // check processing

        if (task.start < time) {
          Log::println("[JOB {}/{}] Overlapping: current time = {}, start time = {}", job, task.operation.index, time, task.start);
          return false;
        }

        time = task.completion;

        if (machine != NoMachine) {
          if (task.machine != machine) {
            Log::println("[JOB {}/{}] Wrong machine: current machine = {}, assigned machine = {}", job, task.operation.index, machine, task.machine);
            return false;
          }
        } else {
          machine = task.machine;
        }

        if (task.operation.index != index) {
          Log::println("[JOB {}/{}] Wrong operation: current index = {}, operation index = {}", job, task.operation.index, index, task.operation.index);
          return false;
        }

        ++index;
      }

      return true;
    }

    bool is_schedule_valid_for_machine(const JobShopSchedule& schedule, MachineId machine)
    {
      auto processing_tasks = schedule.tasks()
        | std::views::filter([machine](const JobShopTask& task) { return task.machine == machine; })
        | std::ranges::to<std::vector<JobShopTask>>();

      std::ranges::stable_sort(processing_tasks, [](const JobShopTask& lhs, const JobShopTask& rhs) {
        return std::tie(lhs.start, lhs.completion) < std::tie(rhs.start, rhs.completion);
      });

      Time time = 0;

      for (const JobShopTask& task :processing_tasks) {

        if (task.start < time) {
          Log::println("[MACHINE {}] Overlapping: current time = {}, start time = {}", machine, time, task.start);
          return false;
        }

        time = task.completion;
      }

      return true;
    }

  }

  bool is_schedule_valid(const JobShopSchedule& schedule, std::size_t job_count, std::size_t machine_count)
  {
    const JobShopSchedule::ConstTaskRange task_range = schedule.tasks();
    std::vector<JobShopTask> tasks(task_range.begin(), task_range.end());

    std::ranges::sort(tasks, [](const JobShopTask& lhs, const JobShopTask& rhs) {
      // also check jobs and operations to handle operations with processing time of 0 (like orb07)
      return std::tie(lhs.completion, lhs.operation.job, lhs.operation.index) < std::tie(rhs.completion, rhs.operation.job, rhs.operation.index);
    });

    if (job_count == 0) {
      for (const JobShopTask& task : tasks) {
        job_count = std::max(job_count, to_index(task.operation.job) + 1);
      }
    }

    if (machine_count == 0) {
      for (const JobShopTask& task : tasks) {
        machine_count = std::max(machine_count, to_index(task.machine) + 1);
      }
    }

    // check jobs

    for (const JobId job : JobRange{ job_count }) {
      if (!is_schedule_valid_for_job(schedule, job)) {
        return false;
      }
    }

    // check machines

    for (const MachineId machine : MachineRange{ machine_count }) { // NOLINT
      if (!is_schedule_valid_for_machine(schedule, machine)) {
        return false;
      }
    }

    return true;
  }

  bool is_schedule_valid_for_instance(const JobShopSchedule& schedule, const JobShopInstance& instance)
  {
    // TODO: check all tasks relatively to instance
    return is_schedule_valid(schedule, instance.job_count(), instance.machine_count());
  }

  bool is_schedule_valid_for_instance(const JobShopSchedule& schedule, const FlexibleJobShopInstance& instance)
  {
    // TODO: check all tasks relatively to instance
    return is_schedule_valid(schedule, instance.job_count(), instance.machine_count());
  }

  JobShopSchedule make_schedule_active(const JobShopSchedule& original_schedule)
  {
    const JobShopSchedule::ConstTaskRange task_range = original_schedule.tasks();
    std::vector<JobShopTask> tasks(task_range.begin(), task_range.end());

    std::ranges::sort(tasks, [](const JobShopTask& lhs, const JobShopTask& rhs) {
      // also check jobs and operations to handle operations with processing time of 0 (like orb07)
      return std::tie(lhs.completion, lhs.operation.job, lhs.operation.index) < std::tie(rhs.completion, rhs.operation.job, rhs.operation.index);
    });

    std::size_t job_count = 0;
    std::size_t machine_count = 0;

    for (const JobShopTask& task : tasks) {
      job_count = std::max(job_count, to_index(task.operation.job) + 1);
      machine_count = std::max(machine_count, to_index(task.machine) + 1);
    }

    std::vector<Time> jobs(job_count, Time{ 0 });
    std::vector<Time> machines(machine_count, Time{ 0 });

    JobShopSchedule schedule;

    for (JobShopTask task : tasks) {
      const std::size_t job_index = to_index(task.operation.job);
      assert(job_index < job_count);
      const std::size_t machine_index = to_index(task.machine);
      assert(machine_index < machine_count);

      const Time start = std::max(jobs[job_index], machines[machine_index]);

      if (start < task.start) {
        const Time difference = task.start - start;
        task.start -= difference;
        task.completion -= difference;
      }

      jobs[job_index] = task.completion;
      machines[machine_index] = task.completion;
      schedule.append(task);
    }

    assert(schedule.task_count() == original_schedule.task_count());
    assert(is_schedule_valid(schedule));
    return schedule;
  }

}
