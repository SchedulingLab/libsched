// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_PARA_SLACK_ALGORITHM_H
#define SCHED_PARA_SLACK_ALGORITHM_H

#include <cassert>

#include <algorithm>
#include <vector>

#include <sched/Api.h>
#include <sched/meta/Instance.h>
#include <sched/para/helper/ParallelJob.h>
#include <sched/para/helper/EarliestFinishTime.h>
#include <sched/para/schedule/ParallelSchedule.h>

namespace sched::para {

  struct SCHED_API SlackAlgorithm {

    template<typename Instance>
    ParallelSchedule operator()(const Instance& instance)
    {
      std::vector<ParallelJob> input;

      for (const JobId job : jobs(instance)) {
        input.push_back({ job, instance.processing_time(job, AnyMachine) });
      }

      assert(!input.empty());

      std::ranges::sort(input, [](const ParallelJob& lhs, const ParallelJob& rhs) {
        return lhs.processing_time > rhs.processing_time;
      });

      std::vector<std::vector<ParallelJob>> groups;
      const std::size_t machine_count = instance.machine_count();
      std::size_t job_count = instance.job_count();
      std::size_t i = 0;

      while (job_count > 0) {
        const std::size_t count = std::min(job_count, machine_count);
        std::vector<ParallelJob> group(&input[i], &input[i] + count);
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

      std::ranges::sort(groups, [slack](const std::vector<ParallelJob>& lhs, const std::vector<ParallelJob>& rhs) {
        assert(lhs.size() == rhs.size());
        return slack(lhs) > slack(rhs);
      });

      input.clear();

      for (auto& group : groups) {
        for (auto job : group) {
          if (job.id != NoJob) {
            input.push_back(job);
          }
        }
      }

      return earliest_finish_time(instance, input);
    }
  };

}

#endif // SCHED_PARA_SLACK_ALGORITHM_H
