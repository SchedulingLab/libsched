#ifndef SCHED_OPERATION_LIST_ENGINE_H
#define SCHED_OPERATION_LIST_ENGINE_H

#include <optional>

#include <sched/common/Api.h>

#include "InputConversion.h"
#include "JobListEngine.h"
#include "JobShopSchedule.h"
#include "OperationListInput.h"

namespace sched::shop {

  struct SCHED_API OperationListEngine {
    using Input = OperationListInput;

    template<typename Instance>
    std::optional<JobShopSchedule> operator()(const Instance& instance, const OperationListInput& input)
    {
      auto maybe_job_list = to_job_list(input, instance);

      if (!maybe_job_list) {
        return std::nullopt;
      }

      const JobListEngine engine;
      return engine(instance, *maybe_job_list);
    }
  };

}

#endif // SCHED_OPERATION_LIST_ENGINE_H
