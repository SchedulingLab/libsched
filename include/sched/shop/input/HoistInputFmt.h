// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_HOIST_INPUT_FMT_H
#define SCHED_SHOP_HOIST_INPUT_FMT_H

#include <ranges>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <sched/IdsFmt.h>
#include <sched/shop/input/HoistEmptyInput.h>

template<>
struct fmt::formatter<sched::shop::HoistEmptyInput> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator { return ctx.end(); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  auto format(const sched::shop::HoistEmptyInput& input, format_context& ctx) const -> format_context::iterator
  {
    const sched::shop::PartitionGroup group(input.length);
    const sched::shop::Partition& partition = group.partition(input.float_index);

    fmt::format_to(ctx.out(), "<");
    auto start = input.machines.begin();

    for (const std::size_t index : partition.indices()) {
      auto end = std::next(input.machines.begin(), static_cast<std::ptrdiff_t>(index));
      fmt::format_to(ctx.out(), "{}", std::ranges::subrange(start, end));
      start = end;
    }

    fmt::format_to(ctx.out(), "{}", std::ranges::subrange(start, std::next(input.machines.begin(), static_cast<std::ptrdiff_t>(partition.length()))));
    return fmt::format_to(ctx.out(), ">");
  }

};

#endif // SCHED_SHOP_HOIST_INPUT_FMT_H
