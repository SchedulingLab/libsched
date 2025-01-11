// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_TRANSPORTATION_FLOAT_LIST_ENGINE_H
#define SCHED_SHOP_TRANSPORTATION_FLOAT_LIST_ENGINE_H

#include <cassert>

#include <optional>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/Instance.h>
#include <sched/meta/input/FloatListInput.h>
#include <sched/shop/helper/JobShopTaskComparator.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/shop/engine/TransportationListEngine.h>
#include <sched/shop/input/TransportationListInput.h>

namespace sched::shop {

  template<typename Comparator>
  struct SCHED_API TransportationFloatListEngine {
    using Input = FloatListInput;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const FloatListInput& input)
    {
      const std::size_t transportation_count = instance.transportation_count();
      TransportationListInput transportation_list;

      for (auto value : input) {
        auto id = static_cast<std::size_t>(value * transportation_count);
        assert(id < transportation_count);
        transportation_list.push_back(TransportationId{ id });
      }

      TransportationListEngine<Comparator> engine;
      return engine(instance, transportation_list);
    }
  };

  using TransportationFloatListEngineEST = TransportationFloatListEngine<JobShopTaskEarliestStartingTime>;
  using TransportationFloatListEngineLST = TransportationFloatListEngine<JobShopTaskLatestStartingTime>;
  using TransportationFloatListEngineEFT = TransportationFloatListEngine<JobShopTaskEarliestFinishTime>;
  using TransportationFloatListEngineLFT = TransportationFloatListEngine<JobShopTaskLatestFinishTime>;
  using TransportationFloatListEngineSPT = TransportationFloatListEngine<JobShopTaskShortestProcessingTime>;
  using TransportationFloatListEngineLPT = TransportationFloatListEngine<JobShopTaskLargestProcessingTime>;

}

#endif // SCHED_SHOP_TRANSPORTATION_LIST_ENGINE_H
