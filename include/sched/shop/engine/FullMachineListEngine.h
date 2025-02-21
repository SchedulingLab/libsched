// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_FULL_MACHINE_LIST_ENGINE_H
#define SCHED_SHOP_FULL_MACHINE_LIST_ENGINE_H

#include <cassert>

#include <sched/shop/engine/FullOperationListEngine.h>
#include <sched/shop/input/InputConversion.h>
#include <sched/meta/input/FloatListInput.h>
#include <sched/meta/input/ThreeInput.h>
#include <sched/types/AssignmentTraits.h>
#include <sched/types/EngineTraits.h>

namespace sched::shop {

  template<typename MachineAssignment, typename TransportationAssignment>
  struct FullMachineListEngine {
    using Input = ThreeInput<typename MachineAssignment::Input, FloatListInput, typename TransportationAssignment::Input>;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input)
    {
      using DelegateEngine = FullOperationListEngine<MachineAssignment, TransportationAssignment>;

      const typename DelegateEngine::Input delegate_input = { input.input0, to_operation_list(input.input1, instance), input.input2 };

      DelegateEngine delegate_engine = {};
      return delegate_engine(instance, delegate_input);
    }
  };

}

namespace sched {

  template<typename MachineAssignment, typename TransportationAssignment>
  struct EngineTraits<shop::FullMachineListEngine<MachineAssignment, TransportationAssignment>> {
    static std::string name()
    {
      using namespace std::literals;
      return "mch_"s + AssignmentTraits<MachineAssignment>::name() + '_' + AssignmentTraits<TransportationAssignment>::name();
    }
  };

}

#endif // SCHED_SHOP_FULL_MACHINE_LIST_ENGINE_H
