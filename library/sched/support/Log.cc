// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/support/Log.h>

#include <chrono>
#include <limits>

#include <fmt/chrono.h>

namespace sched {
  namespace {
    thread_local int g_indent = 0; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
  }

  int Log::current_indent_depth()
  {
    return g_indent;
  }

  void Log::open_scope()
  {
    ++g_indent;
  }

  void Log::close_scope()
  {
    --g_indent;
  }

  void Log::set_max_scope(int max_scope)
  {
    const std::lock_guard<std::mutex> lock(g_log_mutex);
    g_max_scope = max_scope;
  }

  void Log::print_line_string(const std::string& string)
  {
    auto now = std::chrono::system_clock::now();

    const std::lock_guard<std::mutex> lock(g_log_mutex);
    fmt::print("[{}] {}{}\n", now, std::string(g_indent, '\t'), string);
  }

  std::mutex Log::g_log_mutex;
  int Log::g_max_scope = std::numeric_limits<int>::max();

}
