// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_TRANSPORTATION_FLOAT_LIST_ENGINE_H
#define SCHED_SHOP_TRANSPORTATION_FLOAT_LIST_ENGINE_H

#include <cassert>

#include <optional>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/meta/Instance.h>
#include <sched/meta/input/FloatListInput.h>
#include <sched/shop/helper/JobShopTaskComparator.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/shop/engine/TransportationVehicleListEngine.h>
#include <sched/shop/input/VehicleListInput.h>
#include <sched/types/EngineTraits.h>

namespace sched::shop {

  template<typename Comparator>
  struct SCHED_API TransportationFloatListEngine {
    using Input = FloatListInput;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const FloatListInput& input)
    {
      const std::size_t vehicle_count = instance.vehicle_count();
      VehicleListInput vehicle_list;

      for (const double value : input) {
        auto id = static_cast<std::size_t>(value * vehicle_count);
        assert(id < vehicle_count);
        vehicle_list.push_back(VehicleId{ id });
      }

      TransportationVehicleListEngine<Comparator> engine;
      return engine(instance, vehicle_list);
    }
  };

  using TransportationFloatListEngineEST = TransportationFloatListEngine<JobShopTaskEarliestStartingTime>;
  using TransportationFloatListEngineLST = TransportationFloatListEngine<JobShopTaskLatestStartingTime>;
  using TransportationFloatListEngineEFT = TransportationFloatListEngine<JobShopTaskEarliestFinishTime>;
  using TransportationFloatListEngineLFT = TransportationFloatListEngine<JobShopTaskLatestFinishTime>;
  using TransportationFloatListEngineSPT = TransportationFloatListEngine<JobShopTaskShortestProcessingTime>;
  using TransportationFloatListEngineLPT = TransportationFloatListEngine<JobShopTaskLargestProcessingTime>;

}

namespace sched {

  template<typename Comparator>
  struct EngineTraits<shop::TransportationFloatListEngine<Comparator>> {
    static std::string name()
    {
      using namespace std::literals;
      return "ftr_"s + Comparator::name();
    }
  };

}

#endif // SCHED_SHOP_TRANSPORTATION_LIST_ENGINE_H
