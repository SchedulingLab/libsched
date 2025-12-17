// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_FMT_HELPER_H
#define SCHED_FMT_HELPER_H

namespace sched::details {

  template<typename Context>
  constexpr auto parse_empty_context(Context& ctx) -> Context::iterator
  {
    auto pos = ctx.begin();
    while (pos != ctx.end() && *pos != '}') {
      ++pos;
    }
    return pos;
  }

}

#endif // SCHED_FMT_HELPER_H
