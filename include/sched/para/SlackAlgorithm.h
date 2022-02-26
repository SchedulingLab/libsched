#ifndef SCHED_PARA_SLACK_ALGORITHM_H
#define SCHED_PARA_SLACK_ALGORITHM_H

#include <cassert>
#include <algorithm>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Instance.h>

#include "ParallelJob.h"
#include "ParallelSchedule.h"

namespace sched::para {

  struct SCHED_API SlackAlgorithm {

    template<typename Instance>
    ParallelSchedule operator()(const Instance& instance) {
      std::vector<ParallelJob> jobs;

      for (auto job : sched::jobs(instance)) {
        jobs.push_back({ job, instance.processing_time(job, AnyMachine) });
      }

      assert(!jobs.empty());

      std::sort(jobs.begin(), jobs.end(), [](const ParallelJob& lhs, const ParallelJob& rhs) {
        return lhs.processing_time > rhs.processing_time;
      });

      std::vector<std::vector<ParallelJob>> groups;
      std::size_t machine_count = instance.machine_count();
      std::size_t job_count = instance.job_count();
      std::size_t i = 0;

      while (job_count > 0) {
        std::size_t count = std::min(job_count, machine_count);
        std::vector<ParallelJob> group(jobs.begin() + i, jobs.begin() + i + count);
        groups.push_back(std::move(group));
        i += count;
        job_count -= count;
      }

      while (groups.back().size() < machine_count) {
        groups.back().push_back({ NoJob, 0 });
      }

      auto slack = [](const std::vector<ParallelJob>& group) {
        return group.front().processing_time - group.back().processing_time;
      };

      std::sort(groups.begin(), groups.end(), [slack](const std::vector<ParallelJob>& lhs, const std::vector<ParallelJob>& rhs) {
        assert(lhs.size() == rhs.size());
        return slack(lhs) > slack(rhs);
      });

      jobs.clear();

      for (auto & group : groups) {
        for (auto job : group) {
          if (job.id != NoJob) {
            jobs.push_back(job);
          }
        }
      }

      std::vector<Time> machines(machine_count, 0);
      ParallelSchedule schedule;

      for (auto job : jobs) {
        auto it = std::min_element(machines.begin(), machines.end());

        ParallelTask task;
        task.job = job.id;
        task.machine = MachineId{static_cast<std::size_t>(machines.begin() - it)};
        task.start = *it;
        task.completion = task.start + job.processing_time;
        schedule.append(task);

        *it += job.processing_time;
      }

      return schedule;
    }

  };

}

#endif // SCHED_PARA_SLACK_ALGORITHM_H
