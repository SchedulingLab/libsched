// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_PARA_LPT_ALGORITHM_H
#define SCHED_PARA_LPT_ALGORITHM_H

#include <algorithm>
#include <vector>

#include <sched/Api.h>
#include <sched/meta/Instance.h>
#include <sched/para/schedule/ParallelSchedule.h>
#include <sched/para/helper/EarliestFinishTime.h>
#include <sched/para/helper/ParallelJob.h>

namespace sched::para {

  struct SCHED_API LongestProcessingTimeAlgorithm {

    template<typename Instance>
    ParallelSchedule operator()(const Instance& instance)
    {
      std::vector<ParallelJob> input;

      for (const JobId job : jobs(instance)) {
        input.push_back({ job, instance.processing_time(job, AnyMachine) });
      }

      std::ranges::sort(input, [](const ParallelJob& lhs, const ParallelJob& rhs) {
        return lhs.processing_time > rhs.processing_time;
      });

      return earliest_finish_time(instance, input);
    }
  };

  using LptAlgorithm = LongestProcessingTimeAlgorithm;

}

#endif // SCHED_PARA_LPT_ALGORITHM_H
