#ifndef SCHED_PARA_LPT_ALGORITHM_H
#define SCHED_PARA_LPT_ALGORITHM_H

#include <algorithm>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Instance.h>

#include "EarliestFinishTime.h"
#include "ParallelJob.h"
#include "ParallelSchedule.h"

namespace sched::para {

  struct SCHED_API LongestProcessingTimeAlgorithm {

    template<typename Instance>
    ParallelSchedule operator()(const Instance& instance) {
      std::vector<ParallelJob> jobs;

      for (auto job : sched::jobs(instance)) {
        jobs.push_back({ job, instance.processing_time(job, AnyMachine) });
      }

      std::sort(jobs.begin(), jobs.end(), [](const ParallelJob& lhs, const ParallelJob& rhs) {
        return lhs.processing_time > rhs.processing_time;
      });

      const EarliestFinishTime eft;
      return eft(instance, jobs);
    }

  };

  using LptAlgorithm = LongestProcessingTimeAlgorithm;

}

#endif // SCHED_PARA_LPT_ALGORITHM_H
