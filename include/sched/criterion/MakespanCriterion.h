// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_MAKESPAN_CRITERION_H
#define SCHED_MAKESPAN_CRITERION_H

#include <algorithm>

#include <sched/Api.h>
#include <sched/Time.h>
#include <sched/criterion/Comparison.h>

namespace sched {

  struct SCHED_API MakespanCriterion {
    using Fitness = Time;

    template<typename Instance, typename Schedule>
    Fitness operator()([[maybe_unused]] const Instance& instance, const Schedule& schedule)
    {
      Time completion = 0;

      for (auto& task : schedule.tasks()) {
        completion = std::max(completion, task.completion);
      }

      return completion;
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    constexpr Comparison compare(Fitness lhs, Fitness rhs)
    {
      if (lhs < rhs) {
        return Comparison::Better;
      }

      if (lhs > rhs) {
        return Comparison::Worse;
      }

      return Comparison::Equivalent;
    }

    static Fitness worst()
    {
      return TimeMax;
    }
  };

}

#endif // SCHED_MAKESPAN_CRITERION_H
