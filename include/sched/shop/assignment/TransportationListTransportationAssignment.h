// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_TRANSPORTATION_LIST_TRANSPORTATION_ASSIGNMENT_H
#define SCHED_SHOP_TRANSPORTATION_LIST_TRANSPORTATION_ASSIGNMENT_H

#include <cassert>

#include <vector>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/Instance.h>
#include <sched/shop/input/TransportationListInput.h>

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
