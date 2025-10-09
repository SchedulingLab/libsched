// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/helper/JobShopCriticalPath.h>

#include <cassert>

#include <algorithm>
#include <tuple>

namespace sched::shop {

  std::vector<JobShopTask> critical_path(const JobShopSchedule& schedule)
  {
    auto task_range = schedule.tasks();
    std::vector<JobShopTask> tasks(task_range.begin(), task_range.end());

    std::ranges::sort(tasks, [](const JobShopTask& lhs, const JobShopTask& rhs) {
      // also check jobs and operations to handle operations with processing time of 0 (like orb07)
      return std::tie(lhs.completion, lhs.operation.job, lhs.operation.index) > std::tie(rhs.completion, rhs.operation.job, rhs.operation.index);
    });

    const std::size_t n = tasks.size();
    std::vector<JobShopTask> result;
    std::size_t i = 0;
    result.push_back(tasks[i]);

    while (i < n && tasks[i].start > 0) {
      // search for the previous task on the machine

      std::size_t j = i + 1;

      while (j < n && tasks[j].machine != tasks[i].machine) {
        ++j;
      }

      if (j < n && tasks[j].completion == tasks[i].start) {
        assert(tasks[j].machine == tasks[i].machine);
        result.push_back(tasks[j]);
        i = j;
        continue;
      }

      // search for the previous task in the job

      std::size_t k = i + 1;

      while (k < n && tasks[k].operation.job != tasks[i].operation.job) {
        ++k;
      }

      if (k == n) {
        break;
      }

      assert(k < n);
      assert(tasks[k].operation.job == tasks[i].operation.job);
      //       assert(tasks[k].completion == tasks[i].start);
      result.push_back(tasks[k]);
      i = k;
    }

    std::ranges::reverse(result);
    return result;
  }

}
