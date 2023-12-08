#ifndef SCHED_COMMON_IDS_FMT_H
#define SCHED_COMMON_IDS_FMT_H

#include <fmt/format.h>

#include "Ids.h"

template <>
struct fmt::formatter<sched::MachineId> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  constexpr auto parse(format_parse_context& ctx) { return ctx.end(); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  auto format(sched::MachineId id, format_context& ctx) const -> format_context::iterator {
    return format_to(ctx.out(), "{}", static_cast<std::size_t>(id));
  }
};

template <>
struct fmt::formatter<sched::JobId> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  constexpr auto parse(format_parse_context& ctx) { return ctx.end(); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  auto format(sched::JobId id, format_context& ctx) const -> format_context::iterator {
    if (id == sched::AnyJob) {
      return format_to(ctx.out(), "any");
    }

    return format_to(ctx.out(), "{}", static_cast<std::size_t>(id));
  }
};

template <>
struct fmt::formatter<sched::OperationId> {
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.end()) { return ctx.end(); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  auto format(sched::OperationId id, format_context& ctx) const -> format_context::iterator {
    return format_to(ctx.out(), "({}, {})", id.job, id.index);
  }
};

#endif // SCHED_COMMON_IDS_FMT_H
