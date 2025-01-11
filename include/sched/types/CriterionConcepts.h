// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_CRITERION_CONCEPTS_H
#define SCHED_CRITERION_CONCEPTS_H

#include <concepts>

#include <sched/criterion/Comparison.h>

namespace sched::concepts {

  namespace details {

    template<typename C, typename Fitness>
    concept BasicCriterion = requires(C criterion, Fitness f1, Fitness f2) {
      { criterion.compare(f1, f2) } -> std::same_as<Comparison>;
    };

  }

  template<typename C, typename Instance>
  concept CriterionFor = std::semiregular<C> && requires(C criterion, Instance instance) {
    typename C::Fitness;
    // { criterion(instance, schedule) } -> std::same_as<typename C::Fitness>;
    requires details::BasicCriterion<C, typename C::Fitness>;
    { C::worst() } -> std::same_as<typename C::Fitness>;
  };

}

#endif // SCHED_CRITERION_CONCEPTS_H
