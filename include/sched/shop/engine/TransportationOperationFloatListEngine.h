// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_TRANSPORTATION_OPERATION_FLOAT_LIST_ENGINE_H
#define SCHED_SHOP_TRANSPORTATION_OPERATION_FLOAT_LIST_ENGINE_H

#include <cassert>

#include <sched/meta/input/FloatListInput.h>
#include <sched/shop/input/InputConversion.h>
#include <sched/shop/engine/TransportationOperationListEngine.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/meta/input/SplitInput.h>
#include <sched/types/AssignmentTraits.h>
#include <sched/types/EngineTraits.h>

namespace sched::shop {

  template<typename Comparator, typename VehicleAssignment>
  struct TransportationOperationFloatListEngine {
    using Input = SplitInput<FloatListInput, typename VehicleAssignment::Input>;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input)
    {
      using DelegateEngine = TransportationOperationListEngine<Comparator, VehicleAssignment>;

      const typename DelegateEngine::Input delegate_input = { to_operation_list(input.input0, instance), input.input1 };

      DelegateEngine delegate_engine = {};
      return delegate_engine(instance, delegate_input);
    }

  };

}

namespace sched {

  template<typename Comparator, typename VehicleAssignment>
  struct EngineTraits<shop::TransportationOperationFloatListEngine<Comparator, VehicleAssignment>> {
    static std::string name()
    {
      using namespace std::literals;
      return "opf_"s + AssignmentTraits<VehicleAssignment>::name() + '_' + Comparator::name();
    }
  };

}

#endif // SCHED_SHOP_TRANSPORTATION_OPERATION_FLOAT_LIST_ENGINE_H
