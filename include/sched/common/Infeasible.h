#ifndef SCHED_INFEASIBLE_H
#define SCHED_INFEASIBLE_H

#include <cstdint>

namespace sched {

  enum class Infeasible : uint8_t {
    Keep,
    Repair,
  };

}

#endif // SCHED_INFEASIBLE_H
