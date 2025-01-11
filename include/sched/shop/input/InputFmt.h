// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_INPUT_FMT_H
#define SCHED_SHOP_INPUT_FMT_H

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <sched/IdsFmt.h>

#include <sched/shop/input/JobListInput.h>
#include <sched/shop/input/MachineListInput.h>
#include <sched/shop/input/OperationListInput.h>

template<>
struct fmt::formatter<sched::shop::JobListInput> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator { return ctx.end(); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  auto format(const sched::shop::JobListInput& input, format_context& ctx) const -> format_context::iterator
  {
    format_to(ctx.out(), "[");
    format_to(ctx.out(), "{}", fmt::join(input, ", "));
    return format_to(ctx.out(), "]");
  }
};

template<>
struct fmt::formatter<sched::shop::OperationListInput> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator { return ctx.end(); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  auto format(const sched::shop::OperationListInput& input, format_context& ctx) const -> format_context::iterator
  {
    format_to(ctx.out(), "[");
    format_to(ctx.out(), "{}", fmt::join(input, ", "));
    return format_to(ctx.out(), "]");
  }
};

template<>
struct fmt::formatter<sched::shop::MachineListInput> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator { return ctx.end(); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  auto format(const sched::shop::MachineListInput& input, format_context& ctx) const -> format_context::iterator
  {
    format_to(ctx.out(), "[");
    format_to(ctx.out(), "{}", fmt::join(input, "; "));
    return format_to(ctx.out(), "]");
  }
};

#endif // SCHED_SHOP_INPUT_FMT_H
