#ifndef SCHED_SHOP_TRANSPORTATION_OPERATION_LIST_ENGINE_H
#define SCHED_SHOP_TRANSPORTATION_OPERATION_LIST_ENGINE_H

#include <optional>

#include <sched/common/Api.h>

#include "InputConversion.h"
#include "JobShopTransportSchedule.h"
#include "OperationListInput.h"
#include "SplitInput.h"
#include "TransportationJobListEngine.h"

namespace sched::shop {

  template<typename Comparator, typename TransportationAssignment>
  struct SCHED_API TransportationOperationListEngine {
    using Input = TransportSplitInput<OperationListInput, typename TransportationAssignment::Input>;
    using Engine = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input)
    {
      auto maybe_job_list = to_job_list(input.schedule, instance);

      if (!maybe_job_list) {
        return std::nullopt;
      }

      TransportationJobListEngine<Comparator, TransportationAssignment> engine;
      return engine(instance, { *maybe_job_list, input.transport });
    }
  };

}

#endif // SCHED_SHOP_TRANSPORTATION_OPERATION_LIST_ENGINE_H
