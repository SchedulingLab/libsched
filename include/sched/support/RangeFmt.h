// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_RANGE_FMT_H
#define SCHED_RANGE_FMT_H

#include <format>
#include <ranges>

namespace sched::details {

  // formatting range with std requires gcc-15, so use this hack until then
  template<typename Context>
  Context range_format_to(Context ctx, std::ranges::range auto r)
  {
    bool first = true;
    std::format_to(ctx, "[");

    for (auto&& element : r) {
      if (first) {
        first = false;
      } else {
        std::format_to(ctx, ", ");
      }

      std::format_to(ctx, "{}", element);
    }

    return std::format_to(ctx, "]");
  }

}

#endif // SCHED_RANGE_FMT_H
