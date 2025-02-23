// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_FULL_OPERATION_LIST_ENGINE_H
#define SCHED_SHOP_FULL_OPERATION_LIST_ENGINE_H

#include <optional>

#include <sched/shop/input/OperationListInput.h>
#include <sched/shop/engine/FullJobListEngine.h>
#include <sched/shop/input/InputConversion.h>
#include <sched/meta/input/ThreeInput.h>
#include <sched/types/AssignmentTraits.h>
#include <sched/types/EngineTraits.h>

namespace sched::shop {

  template<typename MachineAssignment, typename VehicleAssignment>
  struct FullOperationListEngine {
    using Input = ThreeInput<typename MachineAssignment::Input, OperationListInput, typename VehicleAssignment::Input>;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input)
    {
      auto maybe_job_list = to_job_list(input, instance);

      if (!maybe_job_list) {
        return std::nullopt;
      }

      using DelegateEngine = FullJobListEngine<MachineAssignment, VehicleAssignment>;

      const typename DelegateEngine::Input delegate_input = { input.input0, std::move(*maybe_job_list), input.input2 };

      DelegateEngine delegate_engine = {};
      return delegate_engine(instance, delegate_input);
    }
  };

}

namespace sched {

  template<typename MachineAssignment, typename VehicleAssignment>
  struct EngineTraits<shop::FullOperationListEngine<MachineAssignment, VehicleAssignment>> {
    static std::string name()
    {
      using namespace std::literals;
      return "ope_"s + AssignmentTraits<MachineAssignment>::name() + '_' + AssignmentTraits<VehicleAssignment>::name();
    }
  };

}

#endif // SCHED_SHOP_FULL_OPERATION_LIST_ENGINE_H
