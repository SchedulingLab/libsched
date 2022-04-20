#include <sched/shop/JobShopCriticalPath.h>

#include <cassert>
#include <algorithm>

namespace sched::shop {

  std::vector<JobShopTask> critical_path(const JobShopSchedule& schedule) {
    auto task_range = schedule.tasks();
    std::vector<JobShopTask> tasks(task_range.begin(), task_range.end());

    std::sort(tasks.begin(), tasks.end(), [](const JobShopTask& lhs, const JobShopTask& rhs) {
      return lhs.completion > rhs.completion;
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

      if (j != n && tasks[j].completion == tasks[i].start) {
        result.push_back(tasks[j]);
        i = j;
        continue;
      }

      // search for the previous task in the job

      std::size_t k = i + 1;

      while (k < n && tasks[k].operation.job != tasks[k].operation.job) {
        ++k;
      }

      assert(k < n && tasks[k].completion == tasks[i].start);
      result.push_back(tasks[k]);
      i = k;
    }

    std::reverse(result.begin(), result.end());
    return result;
  }

}
