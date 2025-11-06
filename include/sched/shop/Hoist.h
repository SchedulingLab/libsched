// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_HOIST_H
#define SCHED_SHOP_HOIST_H

#include <cstdint>

#include <sched/Api.h>
#include <sched/Ids.h>

namespace sched::shop {

  struct SCHED_API Move {
    MachineId orig;
    MachineId dest;
  };

  SCHED_API bool operator<(const Move& lhs, const Move& rhs);

  enum class MoveFilter : uint8_t {
    All,
    RemoveTrivial,
  };

}

#endif // SCHED_SHOP_HOIST_H
