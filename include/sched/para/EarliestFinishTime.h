#ifndef SCHED_PARA_EARLIEST_FINISH_TIME_H
#define SCHED_PARA_EARLIEST_FINISH_TIME_H

#include <queue>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Time.h>

#include "ParallelJob.h"
#include "ParallelSchedule.h"

namespace sched::para {

  struct SCHED_API EarliestFinishTime {

    template<typename Instance>
    ParallelSchedule operator()(const Instance& instance, const std::vector<ParallelJob>& jobs)
    {
      std::vector<Time> machines(instance.machine_count(), 0);

      auto comparator = [&machines](std::size_t lhs, std::size_t rhs) {
        return machines[lhs] > machines[rhs];
      };

      std::priority_queue<std::size_t, std::vector<std::size_t>, decltype(comparator)> queue(comparator);

      for (std::size_t i = 0; i < instance.machine_count(); ++i) {
        queue.push(i);
      }

      ParallelSchedule schedule;

      for (auto job : jobs) {
        std::size_t machine = queue.top();
        queue.pop();

        ParallelTask task = {};
        task.job = job.id;
        task.machine = MachineId{ machine };
        task.start = machines[machine];
        task.completion = task.start + job.processing_time;
        schedule.append(task);

        machines[machine] += job.processing_time;
        queue.push(machine);
      }

      return schedule;
    }
  };

}

#endif // SCHED_PARA_EARLIEST_FINISH_TIME_H
