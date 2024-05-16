#ifndef SCHED_SHOP_TRANSPORTATION_H
#define SCHED_SHOP_TRANSPORTATION_H

#include <cstdint>

namespace sched::shop {

  enum class TransportationKind : uint8_t {
    Empty,
    Loaded,
  };

  enum class TransportationMode : uint8_t {
    None,
    Load,
    Unload,
    LoadUnload,
  };

}

#endif // SCHED_SHOP_TRANSPORTATION_H
