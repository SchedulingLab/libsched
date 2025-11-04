// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/Hoist.h>

#include <tuple>

namespace sched::shop {

  bool operator<(const Move& lhs, const Move& rhs)
  {
    return std::tie(lhs.orig, lhs.dest) < std::tie(rhs.orig, rhs.dest);
  }

}
