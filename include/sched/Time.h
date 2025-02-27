// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_TIME_H
#define SCHED_TIME_H

#include <cstdint>

#include <limits>

#include <sched/Api.h>

namespace sched {

  using Time = int64_t;

  constexpr Time TimeMax = std::numeric_limits<Time>::max();

  struct SCHED_API TimeWindow {
    Time lo;
    Time hi;
  };

}

#endif // SCHED_TIME_H
