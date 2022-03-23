#ifndef SCHED_COMMON_IDS_FMT_H
#define SCHED_COMMON_IDS_FMT_H

#include <fmt/format.h>

#include "Ids.h"

template <>
struct fmt::formatter<sched::MachineId> {
  template <typename FormatContext>
  auto format(sched::MachineId id, FormatContext& ctx) -> decltype(ctx.out()) {
    return format_to(ctx.out(), "{}", static_cast<std::size_t>(id));
  }
};

template <>
struct fmt::formatter<sched::JobId> {
  template <typename FormatContext>
  auto format(sched::JobId id, FormatContext& ctx) -> decltype(ctx.out()) {
    return format_to(ctx.out(), "{}", static_cast<std::size_t>(id));
  }
};

template <>
struct fmt::formatter<sched::OperationId> {
  template <typename FormatContext>
  auto format(sched::OperationId id, FormatContext& ctx) -> decltype(ctx.out()) {
    return format_to(ctx.out(), "({},{})", id.job, id.index);
  }
};

#endif // SCHED_COMMON_IDS_FMT_H
