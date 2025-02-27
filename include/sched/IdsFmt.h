// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_IDS_FMT_H
#define SCHED_IDS_FMT_H

#include <fmt/format.h>

#include "Ids.h"

namespace sched {

  inline auto format_as(MachineId id) { return fmt::underlying(id); }
  inline auto format_as(VehicleId id) { return fmt::underlying(id); }

}

template<>
struct fmt::formatter<sched::JobId> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator { return ctx.end(); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  auto format(sched::JobId id, format_context& ctx) const -> format_context::iterator
  {
    if (id == sched::AnyJob) {
      return format_to(ctx.out(), "any");
    }

    return format_to(ctx.out(), "{}", static_cast<std::size_t>(id));
  }
};

template<>
struct fmt::formatter<sched::OperationId> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator { return ctx.end(); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  auto format(sched::OperationId id, format_context& ctx) const -> format_context::iterator
  {
    return format_to(ctx.out(), "({}, {})", id.job, id.index);
  }
};

#endif // SCHED_IDS_FMT_H
