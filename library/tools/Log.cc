#include <sched/tools/Log.h>

#include <chrono>
#include <limits>
#include <thread>

#include <iostream>

#include <fmt/chrono.h>

namespace sched {

  void Log::open_scope() {
    ++g_indent;
  }

  void Log::close_scope() {
    --g_indent;
  }

  void Log::set_max_scope(int max_scope) {
    std::lock_guard<std::mutex> lock(g_log_mutex);
    g_max_scope = max_scope;
  }

  void Log::print_line_string(const std::string& string) {
    auto now = std::chrono::system_clock::now();

    std::lock_guard<std::mutex> lock(g_log_mutex);
    fmt::print("[{}] {}{}\n", now, std::string(g_indent, '\t'), string);
  }

  std::mutex Log::g_log_mutex;
  thread_local int Log::g_indent = 0;
  int Log::g_max_scope = std::numeric_limits<int>::max();

}
