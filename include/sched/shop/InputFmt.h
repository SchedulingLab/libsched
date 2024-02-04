#ifndef SCHED_SHOP_INPUT_FMT_H
#define SCHED_SHOP_INPUT_FMT_H

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <sched/common/IdsFmt.h>

#include "JobListInput.h"
#include "MachineListInput.h"
#include "OperationListInput.h"

template<>
struct fmt::formatter<sched::shop::JobListInput> {
  constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.end()) { return ctx.end(); }

  template<typename FormatContext>
  auto format(const sched::shop::JobListInput& input, FormatContext& ctx) const -> decltype(ctx.out())
  {
    format_to(ctx.out(), "[");
    format_to(ctx.out(), "{}", fmt::join(input, ", "));
    return format_to(ctx.out(), "]");
  }
};

template<>
struct fmt::formatter<sched::shop::OperationListInput> {
  constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.end()) { return ctx.end(); }

  template<typename FormatContext>
  auto format(const sched::shop::OperationListInput& input, FormatContext& ctx) const -> decltype(ctx.out())
  {
    format_to(ctx.out(), "[");
    format_to(ctx.out(), "{}", fmt::join(input, ", "));
    return format_to(ctx.out(), "]");
  }
};

template<>
struct fmt::formatter<sched::shop::MachineListInput> {
  constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.end()) { return ctx.end(); }

  template<typename FormatContext>
  auto format(const sched::shop::MachineListInput& input, FormatContext& ctx) const -> decltype(ctx.out())
  {
    format_to(ctx.out(), "[");
    format_to(ctx.out(), "{}", fmt::join(input, "; "));
    return format_to(ctx.out(), "]");
  }
};

#endif // SCHED_SHOP_INPUT_FMT_H
