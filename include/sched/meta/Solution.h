// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_SOLUTION_H
#define SCHED_SHOP_SOLUTION_H

#include <optional>

#include <sched/types/CriterionConcepts.h>
#include <sched/types/EngineConcepts.h>

namespace sched {

  template<typename Engine, typename Criterion>
  struct BasicSolution {
    using Input = typename Engine::Input;
    using Schedule = std::optional<typename Engine::Schedule>;
    using Fitness = typename Criterion::Fitness;

    Input input;
    Schedule schedule;
    Fitness fitness;
  };

}

#endif // SCHED_SHOP_SOLUTION_H
