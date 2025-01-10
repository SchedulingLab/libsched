#include <sched/shop/schedule/JobShopSchedule.h>

#include <cassert>

#include <algorithm>
#include <tuple>

namespace sched::shop {

  bool is_schedule_valid(const JobShopSchedule& schedule)
  {
    auto task_range = schedule.tasks();
    std::vector<JobShopTask> tasks(task_range.begin(), task_range.end());

    std::sort(tasks.begin(), tasks.end(), [](const JobShopTask& lhs, const JobShopTask& rhs) {
      // also check jobs and operations to handle operations with processing time of 0 (like orb07)
      return std::tie(lhs.completion, lhs.operation.job, lhs.operation.index) < std::tie(rhs.completion, rhs.operation.job, rhs.operation.index);
    });

    std::size_t machine_count = 0;
    std::size_t job_count = 0;

    for (auto& task : tasks) {
      machine_count = std::max(machine_count, to_index(task.machine) + 1);
      job_count = std::max(job_count, to_index(task.operation.job) + 1);
    }

    struct JobState {
      std::size_t operation = 0;
      Time time = 0;
    };

    std::vector<JobState> jobs(job_count, JobState{});
    std::vector<Time> machines(machine_count, Time{ 0 });

    for (auto& task : tasks) {
      auto job_index = to_index(task.operation.job);
      assert(job_index < job_count);
      auto machine_index = to_index(task.machine);
      assert(machine_index < machine_count);

      if (task.start < jobs[job_index].time || task.start < machines[machine_index]) {
        return false;
      }

      jobs[job_index].time = machines[machine_index] = task.completion;

      if (task.operation.index != jobs[job_index].operation) {
        return false;
      }

      ++jobs[job_index].operation;
    }

    return true;
  }

  JobShopSchedule make_schedule_active(const JobShopSchedule& original_schedule)
  {
    auto task_range = original_schedule.tasks();
    std::vector<JobShopTask> tasks(task_range.begin(), task_range.end());

    std::sort(tasks.begin(), tasks.end(), [](const JobShopTask& lhs, const JobShopTask& rhs) {
      // also check jobs and operations to handle operations with processing time of 0 (like orb07)
      return std::tie(lhs.completion, lhs.operation.job, lhs.operation.index) < std::tie(rhs.completion, rhs.operation.job, rhs.operation.index);
    });

    std::size_t job_count = 0;
    std::size_t machine_count = 0;

    for (auto& task : tasks) {
      job_count = std::max(job_count, to_index(task.operation.job) + 1);
      machine_count = std::max(machine_count, to_index(task.machine) + 1);
    }

    std::vector<Time> jobs(job_count, Time{ 0 });
    std::vector<Time> machines(machine_count, Time{ 0 });

    JobShopSchedule schedule;

    for (auto& task : tasks) {
      auto job_index = to_index(task.operation.job);
      assert(job_index < job_count);
      auto machine_index = to_index(task.machine);
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
