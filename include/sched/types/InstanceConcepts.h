// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_INSTANCE_CONCEPT_H
#define SCHED_INSTANCE_CONCEPT_H

#include <cstddef>

#include <concepts>

namespace sched::concepts {

  template<typename I>
  concept Instance = std::semiregular<I> && requires(I instance) {
    { instance.machine_count() } -> std::convertible_to<std::size_t>;
    { instance.job_count() } -> std::convertible_to<std::size_t>;
  };

}

#endif // SCHED_INSTANCE_CONCEPT_H
