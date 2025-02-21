// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_PARA_LISTFIT_ALGORITHM_H
#define SCHED_PARA_LISTFIT_ALGORITHM_H

#include <cassert>

#include <algorithm>
#include <vector>

#include <sched/Api.h>
#include <sched/meta/Instance.h>
#include <sched/criterion/MakespanCriterion.h>
#include <sched/para/algorithm/LptAlgorithm.h>
#include <sched/para/algorithm/MultifitAlgorithm.h>
#include <sched/para/helper/ParallelJob.h>
#include <sched/para/schedule/ParallelSchedule.h>

namespace sched::para {

  struct SCHED_API ListfitAlgorithm {

    template<typename Instance>
    ParallelSchedule operator()(const Instance& instance)
    {
      const LptAlgorithm lpt;
      MultifitAlgorithm multifit;

      ParallelSchedule schedule = lpt(instance);

      const MakespanCriterion criterion;
      Time makespan = criterion(instance, schedule);

      std::vector<ParallelJob> jobs;
      std::vector<ParallelJob> jobs_a;
      std::vector<ParallelJob> jobs_b;
      Time sum = 0;
      Time max = 0;

      for (auto job : sched::jobs(instance)) {
        Time processing_time = instance.processing_time(job, AnyMachine);
        jobs.push_back({ job, processing_time });
        sum += processing_time;
        max = std::max(max, processing_time);
      }

      [[maybe_unused]] std::size_t job_count = instance.job_count();
      assert(job_count == jobs.size());
      jobs_a = std::move(jobs);

      auto sort_by_spt = [](std::vector<ParallelJob>& jobs) {
        std::sort(jobs.begin(), jobs.end(), [](const ParallelJob& lhs, const ParallelJob& rhs) {
          return lhs.processing_time < rhs.processing_time;
        });
      };

      auto sort_by_lpt = [](std::vector<ParallelJob>& jobs) {
        std::sort(jobs.begin(), jobs.end(), [](const ParallelJob& lhs, const ParallelJob& rhs) {
          return lhs.processing_time > rhs.processing_time;
        });
      };

      for (int i = 0; i < 4; ++i) {
        while (!jobs_a.empty()) {
          switch (i) {
            case 0:
              sort_by_spt(jobs_a);
              sort_by_spt(jobs_b);
              break;
            case 1:
              sort_by_lpt(jobs_a);
              sort_by_spt(jobs_b);
              break;
            case 2:
              sort_by_spt(jobs_a);
              sort_by_lpt(jobs_b);
              break;
            case 3:
              sort_by_lpt(jobs_a);
              sort_by_lpt(jobs_b);
              break;
            default:
              assert(false);
              break;
          }

          jobs.clear();
          jobs.insert(jobs.end(), jobs_b.begin(), jobs_b.end());
          jobs.insert(jobs.end(), jobs_a.begin(), jobs_a.end());
          assert(jobs.size() == job_count);

          ParallelSchedule multifit_schedule = multifit.apply(instance, jobs, sum, max);
          Time multifit_makespan = criterion(instance, multifit_schedule);

          if (multifit_makespan < makespan) {
            makespan = multifit_makespan;
            schedule = multifit_schedule;
          }

          jobs_b.push_back(jobs_a.back());
          jobs_a.pop_back();
        }

        jobs_a = std::move(jobs_b);
        assert(jobs_a.size() == job_count);
      }

      return schedule;
    }
  };

}

#endif // SCHED_PARA_LISTFIT_ALGORITHM_H
