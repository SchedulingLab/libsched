// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_PARA_FIRST_FIT_DECREASING_ALGORITHM_H
#define SCHED_PARA_FIRST_FIT_DECREASING_ALGORITHM_H

#include <cassert>

#include <vector>

#include <sched/Time.h>

#include <sched/para/helper/ParallelJob.h>

namespace sched::para {

  // input must be sorted
  inline std::vector<std::vector<ParallelJob>> compute_first_fit_decreasing(const std::vector<ParallelJob>& input, Time maximum)
  {
    std::vector<std::vector<ParallelJob>> boxes;
    std::vector<Time> times;

    for (const auto& job : input) {
      bool packed = false;
      const std::size_t count = boxes.size();

      for (std::size_t i = 0; i < count; ++i) {
        if (times[i] + job.processing_time <= maximum) {
          boxes[i].push_back(job);
          times[i] += job.processing_time;
          packed = true;
          break;
        }
      }

      if (!packed) {
        boxes.push_back({ job });
        times.push_back(job.processing_time);
      }
    }

    return boxes;
  }

}

#endif // SCHED_PARA_FIRST_FIT_DECREASING_ALGORITHM_H
