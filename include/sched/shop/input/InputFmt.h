// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_INPUT_FMT_H
#define SCHED_SHOP_INPUT_FMT_H

#include <format>

#include <sched/IdsFmt.h>

#include <sched/shop/input/JobListInput.h>
#include <sched/shop/input/MachineListInput.h>
#include <sched/shop/input/OperationListInput.h>

template<>
struct std::formatter<sched::shop::JobListInput> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  constexpr auto parse(Context& ctx) -> Context::iterator { return ctx.end(); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  auto format(const sched::shop::JobListInput& input, Context& ctx) const -> Context::iterator
  {
    return std::format_to(ctx.out(), "{}", input);
  }
};

template<>
struct std::formatter<sched::shop::OperationListInput> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  constexpr auto parse(Context& ctx) -> Context::iterator { return ctx.end(); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  auto format(const sched::shop::OperationListInput& input, Context& ctx) const -> Context::iterator
  {
    return std::format_to(ctx.out(), "{}", input);
  }
};

template<>
struct std::formatter<sched::shop::MachineListInput> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  constexpr auto parse(Context& ctx) -> Context::iterator { return ctx.end(); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  auto format(const sched::shop::MachineListInput& input, Context& ctx) const -> Context::iterator
  {
    return std::format_to(ctx.out(), "{}", input);
  }
};

#endif // SCHED_SHOP_INPUT_FMT_H
