#ifndef SCHED_SHOP_GIFFLER_THOMPSON_ENGINE_H
#define SCHED_SHOP_GIFFLER_THOMPSON_ENGINE_H

#include <cassert>

#include <optional>

#include <sched/common/Api.h>

#include "GifflerThompson.h"
#include "InputConversion.h"
#include "JobListInput.h"
#include "JobShopSchedule.h"
#include "MachineListInput.h"
#include "OperationListInput.h"
#include "ShopInstanceConcepts.h"

namespace sched::shop {

  struct SCHED_API OperationListGTEngine {
    using Input = OperationListInput;
    using Schedule = JobShopSchedule;

    template<typename Instance>
      requires(concepts::ShopInstance<Instance>)
    std::optional<JobShopSchedule> operator()(const Instance& instance, const OperationListInput& input)
    {
      auto compare = [&input](const JobShopTask& lhs, const JobShopTask& rhs) {
        auto lhs_iterator = std::find(input.begin(), input.end(), lhs.operation);
        assert(lhs_iterator != input.end());
        auto rhs_iterator = std::find(input.begin(), input.end(), rhs.operation);
        assert(rhs_iterator != input.end());
        return lhs_iterator < rhs_iterator;
      };

      return giffler_thompson(instance, compare);
    }
  };

  struct SCHED_API JobListGTEngine {
    using Input = JobListInput;
    using Schedule = JobShopSchedule;

    template<typename Instance>
      requires(concepts::ShopInstance<Instance>)
    std::optional<JobShopSchedule> operator()(const Instance& instance, const JobListInput& input)
    {
      auto operation_list = to_operation_list(input, instance);

      OperationListGTEngine engine;
      return engine(instance, operation_list);
    }
  };

  struct SCHED_API MachineListGTEngine {
    using Input = MachineListInput;
    using Schedule = JobShopSchedule;

    template<typename Instance>
      requires(concepts::ShopInstance<Instance>)
    std::optional<JobShopSchedule> operator()(const Instance& instance, const MachineListInput& input)
    {
      auto compare = [&input](const JobShopTask& lhs, const JobShopTask& rhs) {
        assert(lhs.machine == rhs.machine);
        const auto& operations = input[lhs.machine];
        auto lhs_iterator = std::find(operations.begin(), operations.end(), lhs.operation);
        assert(lhs_iterator != operations.end());
        auto rhs_iterator = std::find(operations.begin(), operations.end(), rhs.operation);
        assert(rhs_iterator != operations.end());
        return lhs_iterator < rhs_iterator;
      };

      return giffler_thompson(instance, compare);
    }

  };

}

#endif // SCHED_SHOP_GIFFLER_THOMPSON_ENGINE_H
