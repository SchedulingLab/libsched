#ifndef SCHED_COMMON_RAW_ID_H
#define SCHED_COMMON_RAW_ID_H

#include <cstddef>

#include <limits>

namespace sched {

  using RawId = std::size_t;

  constexpr RawId RawNoId = std::numeric_limits<RawId>::max();
  constexpr RawId RawAnyId = RawNoId - 1;

}

#endif // SCHED_COMMON_RAW_ID_H
