#ifndef SCHED_PARA_MULTIFIT_ALGORITHM_H
#define SCHED_PARA_MULTIFIT_ALGORITHM_H

#include <cassert>

#include <algorithm>
#include <vector>

#include <sched/Api.h>
#include <sched/Instance.h>
#include <sched/para/helper/FirstFitDecreasing.h>
#include <sched/para/helper/ParallelJob.h>
#include <sched/para/schedule/ParallelSchedule.h>

namespace sched::para {

  namespace details {

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

  struct SCHED_API MultifitAlgorithm {

    template<typename Instance, int Iterations = 7>
    ParallelSchedule operator()(const Instance& instance)
    {
      std::vector<ParallelJob> jobs;
      Time sum = 0;
      Time max = 0;

      for (auto job : sched::jobs(instance)) {
        Time processing_time = instance.processing_time(job, AnyMachine);
        jobs.push_back({ job, processing_time });
        sum += processing_time;
        max = std::max(max, processing_time);
      }

      std::sort(jobs.begin(), jobs.end(), [](const ParallelJob& lhs, const ParallelJob& rhs) {
        return lhs.processing_time > rhs.processing_time;
      });

      return apply(instance, jobs, sum, max);
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

        auto boxes = details::compute_first_fit_decreasing(jobs, middle);

        if (boxes.size() <= instance.machine_count()) {
          upper = middle;
          result = std::move(boxes);
        } else {
          lower = middle;
        }
      }

      if (result.empty()) {
        result = details::compute_first_fit_decreasing(jobs, upper);
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
