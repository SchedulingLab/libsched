#ifndef SCHED_SHOP_TRANSPORTATION_OPERATION_LIST_ENGINE_H
#define SCHED_SHOP_TRANSPORTATION_OPERATION_LIST_ENGINE_H

#include <optional>

#include <sched/Api.h>

#include <sched/shop/input/InputConversion.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/shop/input/OperationListInput.h>
#include <sched/meta/input/SplitInput.h>
#include <sched/shop/engine/TransportationJobListEngine.h>

namespace sched::shop {

  template<typename Comparator, typename TransportationAssignment>
  struct SCHED_API TransportationOperationListEngine {
    using Input = SplitInput<OperationListInput, typename TransportationAssignment::Input>;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input)
    {
      auto maybe_job_list = to_job_list(input.input0, instance);

      if (!maybe_job_list) {
        return std::nullopt;
      }

      TransportationJobListEngine<Comparator, TransportationAssignment> engine;
      return engine(instance, { *maybe_job_list, input.input1 });
    }
  };

}

#endif // SCHED_SHOP_TRANSPORTATION_OPERATION_LIST_ENGINE_H
