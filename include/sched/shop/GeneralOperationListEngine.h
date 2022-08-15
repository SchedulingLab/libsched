#ifndef SCHED_SHOP_GENERAL_OPERATION_LIST_ENGINE_H
#define SCHED_SHOP_GENERAL_OPERATION_LIST_ENGINE_H

#include <optional>
#include <vector>

#include <sched/common/Api.h>

#include "InputConversion.h"
#include "GeneralJobShopSchedule.h"
#include "GeneralJobListEngine.h"
#include "OperationListInput.h"

namespace sched::shop {

  struct SCHED_API GeneralOperationListEngine {
    using Input = OperationListInput;

    template<typename Instance>
    std::optional<GeneralJobShopSchedule> operator()(const Instance& instance, const OperationListInput& input) {
      auto maybe_job_list = to_job_list(input, instance);

      if (!maybe_job_list) {
        return std::nullopt;
      }

      GeneralJobListEngine engine;
      return engine(instance, *maybe_job_list);
    }
  };

}

#endif // SCHED_SHOP_GENERAL_OPERATION_LIST_ENGINE_H
