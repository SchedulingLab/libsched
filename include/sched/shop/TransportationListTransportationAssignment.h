#ifndef SCHED_SHOP_TRANSPORTATION_LIST_TRANSPORTATION_ASSIGNMENT_H
#define SCHED_SHOP_TRANSPORTATION_LIST_TRANSPORTATION_ASSIGNMENT_H

#include <cassert>

#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>

#include "TransportationListInput.h"

namespace sched::shop {

  struct SCHED_API TransportationListTransportationAssignment {
    using Input = TransportationListInput;

    template<typename Instance>
    std::vector<TransportationId> operator()([[maybe_unused]] const Instance& instance, const TransportationListInput& input)
    {
      return input;
    }
  };

}

#endif // SCHED_SHOP_TRANSPORTATION_LIST_TRANSPORTATION_ASSIGNMENT_H
