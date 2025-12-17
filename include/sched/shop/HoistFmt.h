// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_HOIST_FMT_H
#define SCHED_SHOP_HOIST_FMT_H

#include <format>

#include <sched/IdsFmt.h>
#include <sched/shop/Hoist.h>
#include <sched/support/FmtHelper.h>

template<>
struct std::formatter<sched::shop::Move> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  constexpr auto parse(Context& ctx) -> Context::iterator { return sched::details::parse_empty_context(ctx); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  auto format(const sched::shop::Move& move, Context& ctx) const -> Context::iterator
  {
    return std::format_to(ctx.out(), "({}, {})", move.orig, move.dest);
  }

};

#endif // SCHED_SHOP_HOIST_FMT_H
