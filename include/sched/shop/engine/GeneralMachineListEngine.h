// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_GENERAL_MACHINE_LIST_ENGINE_H
#define SCHED_SHOP_GENERAL_MACHINE_LIST_ENGINE_H

#include <cassert>

#include <algorithm>
#include <optional>
#include <tuple>
#include <vector>

#include <sched/Ids.h>
#include <sched/meta/Instance.h>
#include <sched/meta/input/FloatListInput.h>
#include <sched/shop/helper/JobShopTaskComparator.h>
#include <sched/shop/helper/JobShopTransportStates.h>
#include <sched/shop/input/InputConversion.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/types/EngineTraits.h>
#include <sched/types/ShopInstanceConcepts.h>
#include "sched/shop/engine/GeneralOperationListEngine.h"

namespace sched::shop {

  template<typename Comparator>
  struct GeneralMachineListEngine {
    using Input = FloatListInput;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
      requires(concepts::ShopTransportInstance<Instance>)
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const FloatListInput& input)
    {
      auto operation_list = to_operation_list(input, instance);

      GeneralOperationListEngine<Comparator> engine;
      return engine(instance, operation_list);
    }
  };

  using GeneralMachineListEngineEST = GeneralMachineListEngine<JobShopTaskEarliestStartingTime>;
  using GeneralMachineListEngineLST = GeneralMachineListEngine<JobShopTaskLatestStartingTime>;
  using GeneralMachineListEngineEFT = GeneralMachineListEngine<JobShopTaskEarliestFinishTime>;
  using GeneralMachineListEngineLFT = GeneralMachineListEngine<JobShopTaskLatestFinishTime>;
  using GeneralMachineListEngineSPT = GeneralMachineListEngine<JobShopTaskShortestProcessingTime>;
  using GeneralMachineListEngineLPT = GeneralMachineListEngine<JobShopTaskLargestProcessingTime>;

}

namespace sched {

  template<typename Comparator>
  struct EngineTraits<shop::GeneralMachineListEngine<Comparator>> {
    static std::string name()
    {
      using namespace std::literals;
      return "mch_"s + Comparator::name();
    }
  };

}

#endif // SCHED_SHOP_GENERAL_MACHINE_LIST_ENGINE_H
