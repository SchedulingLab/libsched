#ifndef SCHED_SHOP_TRANSPORTATION_FLOAT_LIST_ENGINE_H
#define SCHED_SHOP_TRANSPORTATION_FLOAT_LIST_ENGINE_H

#include <cassert>
#include <optional>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>
#include <sched/common/Instance.h>

#include "FloatListInput.h"
#include "JobShopTransportSchedule.h"
#include "JobShopTaskComparator.h"
#include "TransportationListEngine.h"
#include "TransportationListInput.h"

namespace sched::shop {

  template<typename Comparator>
  struct SCHED_API TransportationFloatListEngine {
    using Input = FloatListInput;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const FloatListInput& input) {
      const std::size_t transportation_count = instance.transportation_count();
      TransportationListInput transportation_list;

      for (auto value : input) {
        auto id = static_cast<std::size_t>(value * transportation_count);
        assert(id < transportation_count);
        transportation_list.push_back(TransportationId{id});
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
