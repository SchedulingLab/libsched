// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_IDS_FMT_H
#define SCHED_IDS_FMT_H

#include <format>
#include <utility>

#include <sched/Ids.h>
#include <sched/support/FmtHelper.h>

template<>
struct std::formatter<sched::MachineId>
{
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  constexpr auto parse(Context& ctx) -> Context::iterator { return sched::details::parse_empty_context(ctx); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  auto format(sched::MachineId id, Context& ctx) const -> Context::iterator
  {
    return std::format_to(ctx.out(), "{}", std::to_underlying(id));
  }
};

template<>
struct std::formatter<sched::VehicleId>
{
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  constexpr auto parse(Context& ctx) -> Context::iterator { return sched::details::parse_empty_context(ctx); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  auto format(sched::VehicleId id, Context& ctx) const -> Context::iterator
  {
    return std::format_to(ctx.out(), "{}", std::to_underlying(id));
  }
};

template<>
struct std::formatter<sched::JobId> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  constexpr auto parse(Context& ctx) -> Context::iterator { return sched::details::parse_empty_context(ctx); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  auto format(sched::JobId id, Context& ctx) const -> Context::iterator
  {
    if (id == sched::AnyJob) {
      return std::format_to(ctx.out(), "any");
    }

    return std::format_to(ctx.out(), "{}", std::to_underlying(id));
  }
};

template<>
struct std::formatter<sched::OperationId> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  constexpr auto parse(Context& ctx) -> Context::iterator { return sched::details::parse_empty_context(ctx); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  auto format(sched::OperationId id, Context& ctx) const -> Context::iterator
  {
    return std::format_to(ctx.out(), "({}, {})", id.job, id.index);
  }
};

#endif // SCHED_IDS_FMT_H
