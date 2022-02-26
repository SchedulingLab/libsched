#ifndef SCHED_MAKESPAN_CRITERION_H
#define SCHED_MAKESPAN_CRITERION_H

#include <algorithm>

#include "Api.h"
#include "Time.h"

namespace sched {

  struct SCHED_API MakespanCriterion {
    using Fitness = Time;

    template<typename Instance, typename Schedule>
    Fitness operator()([[maybe_unused]] const Instance& instance, const Schedule& sched) {
      Time completion = 0;

      for (auto & work : sched) {
        completion = std::max(completion, work.completion);
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
