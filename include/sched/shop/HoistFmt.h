// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_HOIST_FMT_H
#define SCHED_SHOP_HOIST_FMT_H

#include <fmt/core.h>

#include <sched/IdsFmt.h>
#include <sched/shop/Hoist.h>

template<>
struct fmt::formatter<sched::shop::Move> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator { return ctx.end(); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  auto format(const sched::shop::Move& move, format_context& ctx) const -> format_context::iterator
  {
    return fmt::format_to(ctx.out(), "({}, {})", move.orig, move.dest);
  }

};

#endif // SCHED_SHOP_HOIST_FMT_H
