// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_PARA_COMBINE_ALGORITHM_H
#define SCHED_PARA_COMBINE_ALGORITHM_H

#include <cassert>

#include <algorithm>
#include <vector>

#include <sched/Api.h>
#include <sched/meta/Instance.h>
#include <sched/criterion/MakespanCriterion.h>
#include <sched/para/algorithm/LptAlgorithm.h>
#include <sched/para/helper/FirstFitDecreasing.h>
#include <sched/para/helper/ParallelJob.h>
#include <sched/para/schedule/ParallelSchedule.h>

namespace sched::para {

  struct SCHED_API CombineAlgorithm {

    template<typename Instance>
    ParallelSchedule operator()(const Instance& instance)
    {
      const LptAlgorithm lpt;

      ParallelSchedule fallback = lpt(instance);

      const MakespanCriterion criterion;
      Time makespan = criterion(instance, fallback);

      std::vector<ParallelJob> input;
      Time sum = 0;
      Time max = 0;

      for (auto job : jobs(instance)) {
        const Time processing_time = instance.processing_time(job, AnyMachine);
        input.push_back({ job, processing_time });
        sum += processing_time;
        max = std::max(max, processing_time);
      }

      const Time average = sum /= instance.machine_count();

      if (makespan >= 3 * average / 2) {
        return fallback;
      }

      const double ratio = (4.0 / 3.0) - (1.0 / (3.0 * instance.machine_count()));

      Time upper = makespan;
      Time lower = std::max({ average, max, static_cast<Time>(static_cast<double>(makespan) / ratio) });

      std::ranges::sort(input, [](const ParallelJob& lhs, const ParallelJob& rhs) {
        return lhs.processing_time > rhs.processing_time;
      });

      std::vector<std::vector<ParallelJob>> result;

      const Time max_difference = std::max(Time(1), average / 200);

      while ((upper - lower) > max_difference) {
        const Time middle = lower + ((upper - lower) / 2);

        auto boxes = compute_first_fit_decreasing(input, middle);

        if (boxes.size() <= instance.machine_count()) {
          upper = middle;
          result = std::move(boxes);
        } else {
          lower = middle;
        }
      }

      if (result.empty()) {
        return fallback;
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

#endif // SCHED_PARA_COMBINE_ALGORITHM_H
