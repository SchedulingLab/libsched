// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_GIFFLER_THOMPSON_ENGINE_H
#define SCHED_SHOP_GIFFLER_THOMPSON_ENGINE_H

#include <cassert>

#include <algorithm>
#include <optional>

#include <sched/Api.h>
#include <sched/shop/helper/GifflerThompson.h>
#include <sched/shop/input/InputConversion.h>
#include <sched/shop/input/JobListInput.h>
#include <sched/shop/input/MachineListInput.h>
#include <sched/shop/input/OperationListInput.h>
#include <sched/shop/schedule/JobShopSchedule.h>
#include <sched/types/ShopInstanceConcepts.h>

namespace sched::shop {

  struct SCHED_API OperationListGTEngine {
    using Input = OperationListInput;
    using Schedule = JobShopSchedule;

    template<typename Instance>
      requires(concepts::ShopInstance<Instance>)
    std::optional<JobShopSchedule> operator()(const Instance& instance, const OperationListInput& input)
    {
      auto compare = [&input](const JobShopTask& lhs, const JobShopTask& rhs) {
        auto lhs_iterator = std::ranges::find(input, lhs.operation);
        assert(lhs_iterator != input.end());
        auto rhs_iterator = std::ranges::find(input, rhs.operation);
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
        auto lhs_iterator = std::ranges::find(operations, lhs.operation);
        assert(lhs_iterator != operations.end());
        auto rhs_iterator = std::ranges::find(operations, rhs.operation);
        assert(rhs_iterator != operations.end());
        return lhs_iterator < rhs_iterator;
      };

      return giffler_thompson(instance, compare);
    }

  };

}

#endif // SCHED_SHOP_GIFFLER_THOMPSON_ENGINE_H
