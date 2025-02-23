// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_PARA_MULTIFIT_ALGORITHM_H
#define SCHED_PARA_MULTIFIT_ALGORITHM_H

#include <cassert>

#include <algorithm>
#include <vector>

#include <sched/Api.h>
#include <sched/meta/Instance.h>
#include <sched/para/helper/FirstFitDecreasing.h>
#include <sched/para/helper/ParallelJob.h>
#include <sched/para/schedule/ParallelSchedule.h>

namespace sched::para {

  struct SCHED_API MultifitAlgorithm {

    template<typename Instance, int Iterations = 7>
    ParallelSchedule operator()(const Instance& instance)
    {
      std::vector<ParallelJob> input;
      Time sum = 0;
      Time max = 0;

      for (const JobId job : jobs(instance)) {
        const Time processing_time = instance.processing_time(job, AnyMachine);
        input.push_back({ job, processing_time });
        sum += processing_time;
        max = std::max(max, processing_time);
      }

      std::ranges::sort(input, [](const ParallelJob& lhs, const ParallelJob& rhs) {
        return lhs.processing_time > rhs.processing_time;
      });

      return apply(instance, input, sum, max);
    }

    template<typename Instance, int Iterations = 7>
    ParallelSchedule apply(const Instance& instance, const std::vector<ParallelJob>& jobs, Time sum, Time max)
    {
      const Time average = sum / instance.machine_count();
      Time lower = std::max(average, max);
      Time upper = std::max(2 * average, max);

      std::vector<std::vector<ParallelJob>> result;

      for (int i = 0; i < Iterations; ++i) {
        const Time middle = lower + ((upper - lower) / 2);

        auto boxes = compute_first_fit_decreasing(jobs, middle);

        if (boxes.size() <= instance.machine_count()) {
          upper = middle;
          result = std::move(boxes);
        } else {
          lower = middle;
        }
      }

      if (result.empty()) {
        result = compute_first_fit_decreasing(jobs, upper);
        assert(result.size() <= instance.machine_count());
      }

      ParallelSchedule schedule;

      std::size_t machine = 0;

      for (auto& group : result) {
        Time time = 0;

        for (auto& job : group) {
          ParallelTask task = {};
          task.job = job.id;
          task.machine = MachineId{ machine };
          task.start = time;
          task.completion = task.start + job.processing_time;
          schedule.append(task);

          time += job.processing_time;
        }

        ++machine;
      }

      return schedule;
    }
  };

}

#endif // SCHED_PARA_MULTIFIT_ALGORITHM_H
