#ifndef SCHED_SHOP_FLOAT_LIST_TRANSPORTATION_ASSIGNMENT_H
#define SCHED_SHOP_FLOAT_LIST_TRANSPORTATION_ASSIGNMENT_H

#include <cassert>

#include <vector>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/Instance.h>
#include <sched/meta/input/FloatListInput.h>

namespace sched::shop {

  struct SCHED_API FloatListTransportationAssignment {
    using Input = FloatListInput;

    template<typename Instance>
    std::vector<TransportationId> operator()(const Instance& instance, const FloatListInput& input)
    {
      const std::size_t transportation_count = instance.transportation_count();
      std::vector<TransportationId> assignment;

      for (auto value : input) {
        auto id = static_cast<std::size_t>(value * transportation_count);
        assert(id < transportation_count);
        assignment.push_back(TransportationId{ id });
      }

      return assignment;
    }
  };

}

#endif // SCHED_SHOP_FLOAT_LIST_TRANSPORTATION_ASSIGNMENT_H
