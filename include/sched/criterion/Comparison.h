// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_COMPARISON_H
#define SCHED_COMPARISON_H

#include <cstdint>

namespace sched {

  enum class Comparison : uint8_t {
    Better,
    Equivalent,
    Worse,
  };

}

#endif // SCHED_COMPARISON_H
