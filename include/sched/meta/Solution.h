// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_SOLUTION_H
#define SCHED_SHOP_SOLUTION_H

#include <optional>

#include <sched/types/EngineConcepts.h>
#include <sched/types/CriterionConcepts.h>

namespace sched {

  template<typename Engine, typename Criterion, typename Instance>
    requires(concepts::EngineFor<Engine, Instance> && concepts::CriterionFor<Criterion, Instance>)
  struct Solution {
    using Input = typename Engine::Input;
    using Schedule = std::optional<typename Engine::Schedule>;
    using Fitness = typename Criterion::Fitness;

    Input input;
    Schedule schedule;
    Fitness fitness;
  };

}

#endif // SCHED_SHOP_SOLUTION_H
