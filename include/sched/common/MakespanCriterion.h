#ifndef SCHED_MAKESPAN_CRITERION_H
#define SCHED_MAKESPAN_CRITERION_H

#include <algorithm>

#include "Api.h"
#include "Time.h"

namespace sched {

  struct SCHED_API MakespanCriterion {
    using Fitness = Time;

    template<typename Instance, typename Schedule>
    Fitness operator()([[maybe_unused]] const Instance& instance, const Schedule& schedule) {
      Time completion = 0;

      for (auto & task : schedule.tasks()) {
        completion = std::max(completion, task.completion);
      }

      return completion;
    }

    constexpr
    bool is_better(Fitness prev, Fitness next) {
      return next <= prev;
    }

    constexpr
    bool is_strictly_better(Fitness prev, Fitness next) {
      return next < prev;
    }

  };

}

#endif // SCHED_MAKESPAN_CRITERION_H
