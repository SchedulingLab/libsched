#ifndef SCHED_LOG_H
#define SCHED_LOG_H

#include <mutex>

#include <fmt/core.h>

#include <sched/common/Api.h>

namespace sched {

  struct SCHED_API Log {
    Log() = delete;

    template<typename ... T>
    static void println(fmt::format_string<T...> fmt, T&&... args) {
      if (g_indent > g_max_scope) {
        return;
      }

      auto string = fmt::format(fmt, std::forward<T>(args)...);
      print_line_string(string);
    }

    static void open_scope();
    static void close_scope();

    static void set_max_scope(int max_scope);

  private:
    static void print_line_string(const std::string& string);

  private:
    static std::mutex g_log_mutex;
    static thread_local int g_indent;
    static int g_max_scope;
  };

  struct SCHED_API LogScope {
    LogScope() {
      Log::open_scope();
    }

    LogScope(const LogScope&) = delete;

    ~LogScope() {
      Log::close_scope();
    }

    LogScope& operator=(const LogScope&) = delete;
  };

}


#endif // SCHED_LOG_H
