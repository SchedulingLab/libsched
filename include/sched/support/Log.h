// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_LOG_H
#define SCHED_LOG_H

#include <mutex>

#include <fmt/core.h>

#include <sched/Api.h>

namespace sched {

  struct SCHED_API Log {
    Log() = delete;

    template<typename... T>
    static void println(fmt::format_string<T...> fmt, T&&... args)
    {
      if (current_indent_depth() > g_max_scope) {
        return;
      }

      auto string = fmt::format(fmt, std::forward<T>(args)...);
      print_line_string(string);
    }

    static int current_indent_depth();
    static void open_scope();
    static void close_scope();

    static void set_max_scope(int max_scope);

  private:
    static void print_line_string(const std::string& string);

    static std::mutex g_log_mutex;
    static int g_max_scope;
  };

  struct SCHED_API LogScope {
    LogScope()
    {
      Log::open_scope();
    }

    LogScope(const LogScope&) = delete;
    LogScope(LogScope&&) = delete;

    ~LogScope()
    {
      Log::close_scope();
    }

    LogScope& operator=(const LogScope&) = delete;
    LogScope& operator=(LogScope&&) = delete;
  };

}

#endif // SCHED_LOG_H
