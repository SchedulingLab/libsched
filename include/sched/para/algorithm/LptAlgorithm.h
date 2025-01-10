#ifndef SCHED_PARA_LPT_ALGORITHM_H
#define SCHED_PARA_LPT_ALGORITHM_H

#include <algorithm>
#include <vector>

#include <sched/Api.h>
#include <sched/Instance.h>
#include <sched/para/schedule/ParallelSchedule.h>
#include <sched/para/helper/EarliestFinishTime.h>
#include <sched/para/helper/ParallelJob.h>

namespace sched::para {

  struct SCHED_API LongestProcessingTimeAlgorithm {

    template<typename Instance>
    ParallelSchedule operator()(const Instance& instance)
    {
      std::vector<ParallelJob> jobs;

      for (auto job : sched::jobs(instance)) {
        jobs.push_back({ job, instance.processing_time(job, AnyMachine) });
      }

      std::sort(jobs.begin(), jobs.end(), [](const ParallelJob& lhs, const ParallelJob& rhs) {
        return lhs.processing_time > rhs.processing_time;
      });

      return earliest_finish_time(instance, jobs);
    }
  };

  using LptAlgorithm = LongestProcessingTimeAlgorithm;

}

#endif // SCHED_PARA_LPT_ALGORITHM_H
