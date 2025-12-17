// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_BENCHMARK_FMT_H
#define SCHED_SHOP_BENCHMARK_FMT_H

#include <format>

#include <sched/shop/instance/JobShopBenchmark.h>
#include <sched/shop/instance/FlexibleJobShopBenchmark.h>
#include <sched/shop/instance/FlexibleJobShopTransportBenchmark.h>

template<>
struct std::formatter<sched::shop::JobShopBenchmark> : std::formatter<std::string>
{
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  auto format(const sched::shop::JobShopBenchmark& benchmark, Context& ctx) const -> Context::iterator
  {
    return std::formatter<std::string>::format(benchmark.name, ctx);
  }
};

template<>
struct std::formatter<sched::shop::FlexibleJobShopBenchmark> : std::formatter<std::string>
{
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  auto format(const sched::shop::FlexibleJobShopBenchmark& benchmark, Context& ctx) const -> Context::iterator
  {
    return std::formatter<std::string>::format(benchmark.name, ctx);
  }
};

template<>
struct std::formatter<sched::shop::FlexibleJobShopTransportBenchmark> : std::formatter<std::string>
{
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  template<typename Context>
  auto format(const sched::shop::FlexibleJobShopTransportBenchmark& benchmark, Context& ctx) const -> Context::iterator
  {
    return std::formatter<std::string>::format(benchmark.name, ctx);
  }
};

#endif // SCHED_SHOP_BENCHMARK_FMT_H
