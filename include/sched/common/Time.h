#ifndef SCHED_TIME_H
#define SCHED_TIME_H

#include <cstdint>
#include <limits>

namespace sched {

  using Time = int64_t;

  constexpr Time TimeMax = std::numeric_limits<Time>::max();

  struct TimeWindow {
    Time lo;
    Time hi;
  };

}

#endif // SCHED_TIME_H
