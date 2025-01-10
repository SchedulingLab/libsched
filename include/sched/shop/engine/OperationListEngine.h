#ifndef SCHED_OPERATION_LIST_ENGINE_H
#define SCHED_OPERATION_LIST_ENGINE_H

#include <optional>

#include <sched/Api.h>
#include <sched/shop/input/InputConversion.h>
#include <sched/shop/input/OperationListInput.h>
#include <sched/shop/engine/JobListEngine.h>
#include <sched/shop/schedule/JobShopSchedule.h>
#include <sched/types/ShopInstanceConcepts.h>

namespace sched::shop {

  struct SCHED_API OperationListEngine {
    using Input = OperationListInput;
    using Schedule = JobShopSchedule;

    template<typename Instance>
      requires(concepts::ShopInstance<Instance>)
    std::optional<JobShopSchedule> operator()(const Instance& instance, const OperationListInput& input)
    {
      auto maybe_job_list = to_job_list(input, instance);

      if (!maybe_job_list) {
        return std::nullopt;
      }

      JobListEngine engine;
      return engine(instance, *maybe_job_list);
    }
  };

}

#endif // SCHED_OPERATION_LIST_ENGINE_H
