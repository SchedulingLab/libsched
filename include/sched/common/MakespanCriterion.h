#ifndef SCHED_MAKESPAN_CRITERION_H
#define SCHED_MAKESPAN_CRITERION_H

#include <algorithm>

#include "Api.h"
#include "Comparison.h"
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

    constexpr Comparison compare(Fitness lhs, Fitness rhs) {
      if (lhs < rhs) {
        return Comparison::Better;
      }

      if (lhs > rhs) {
        return Comparison::Worse;
      }

      return Comparison::Equivalent;
    }

    static Fitness worst() {
      return TimeMax;
    }

  };

}

#endif // SCHED_MAKESPAN_CRITERION_H
