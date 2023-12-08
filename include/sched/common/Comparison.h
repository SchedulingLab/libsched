#ifndef SCHED_COMPARISON_H
#define SCHED_COMPARISON_H

#include <cstdint>

namespace sched {

  enum class Comparison : uint8_t {
    Better,
    Equivalent,
    Worse,
  };

}

#endif // SCHED_COMPARISON_H
