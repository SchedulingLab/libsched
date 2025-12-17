// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_RANGE_FMT_H
#define SCHED_RANGE_FMT_H

#include <format>
#include <vector>

template <typename T>
struct std::formatter<std::vector<T>> {
  template<typename Context>
  constexpr auto parse(Context& ctx) -> Context::iterator { return ctx.end(); }

  template <typename Context>
  auto format(const std::vector<T>& r, Context& ctx) const -> Context::iterator {
    bool first = true;
    std::format_to(ctx.out(), "[");

    for (auto&& element : r) {
      if (first) {
        first = false;
      } else {
        std::format_to(ctx.out(), ", ");
      }

      std::format_to(ctx.out(), "{}", element);
    }

    return std::format_to(ctx.out(), "]");
  }
};

#endif // SCHED_RANGE_FMT_H
