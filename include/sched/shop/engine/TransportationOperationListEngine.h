// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_TRANSPORTATION_OPERATION_LIST_ENGINE_H
#define SCHED_SHOP_TRANSPORTATION_OPERATION_LIST_ENGINE_H

#include <optional>

#include <sched/Api.h>

#include <sched/shop/input/InputConversion.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/shop/input/OperationListInput.h>
#include <sched/meta/input/SplitInput.h>
#include <sched/shop/engine/TransportationJobListEngine.h>
#include <sched/types/AssignmentTraits.h>
#include <sched/types/EngineTraits.h>

namespace sched::shop {

  template<typename Comparator, typename VehicleAssignment>
  struct SCHED_API TransportationOperationListEngine {
    using Input = SplitInput<OperationListInput, typename VehicleAssignment::Input>;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input)
    {
      auto maybe_job_list = to_job_list(input.input0, instance);

      if (!maybe_job_list) {
        return std::nullopt;
      }

      using DelegateEngine = TransportationJobListEngine<Comparator, VehicleAssignment>;

      const typename DelegateEngine::Input delegate_input = { std::move(*maybe_job_list), input.input1 };

      DelegateEngine delegate_engine;
      return delegate_engine(instance, delegate_input);
    }
  };

}

namespace sched {

  template<typename Comparator, typename VehicleAssignment>
  struct EngineTraits<shop::TransportationOperationListEngine<Comparator, VehicleAssignment>> {
    static std::string name()
    {
      using namespace std::literals;
      return "ope_"s + AssignmentTraits<VehicleAssignment>::name() + '_' + Comparator::name();
    }
  };

}

#endif // SCHED_SHOP_TRANSPORTATION_OPERATION_LIST_ENGINE_H
