// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_HOIST_INPUT_FMT_H
#define SCHED_SHOP_HOIST_INPUT_FMT_H

#include <cassert>

#include <format>

#include <sched/IdsFmt.h>
#include <sched/shop/input/HoistEmptyInput.h>
#include <sched/support/RangeFmt.h>
#include <sched/support/FmtHelper.h>

namespace details {

  inline std::vector<sched::MachineId> extract_cycle(const sched::shop::HoistEmptyInput& input, std::size_t i, std::size_t j)
  {
    assert(i < input.machines.size());
    assert(j <= input.machines.size());
    return { input.machines.data() + i, input.machines.data() + j };
  }

}

template<>
struct std::formatter<sched::shop::HoistEmptyInput> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  constexpr auto parse(Context& ctx) -> Context::iterator { return sched::details::parse_empty_context(ctx); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  auto format(const sched::shop::HoistEmptyInput& input, Context& ctx) const -> Context::iterator
  {
    const sched::shop::PartitionGroup group(input.length);
    const sched::shop::Partition& partition = group.partition(input.float_index);

    std::format_to(ctx.out(), "<");
    std::size_t start = 0;

    for (const std::size_t index : partition.indices()) {
      const std::vector<sched::MachineId> cycle = details::extract_cycle(input, start, index);
      std::format_to(ctx.out(), "{}", cycle);
      start = index;
    }

    const std::vector<sched::MachineId> cycle = details::extract_cycle(input, start, partition.length());
    std::format_to(ctx.out(), "{}", cycle);
    return std::format_to(ctx.out(), ">");
  }

};

#endif // SCHED_SHOP_HOIST_INPUT_FMT_H
