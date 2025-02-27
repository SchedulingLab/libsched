// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_GENERAL_OPERATION_LIST_ENGINE_H
#define SCHED_SHOP_GENERAL_OPERATION_LIST_ENGINE_H

#include <optional>

#include <sched/Api.h>
#include <sched/shop/engine/GeneralJobListEngine.h>
#include <sched/shop/input/InputConversion.h>
#include <sched/shop/input/OperationListInput.h>
#include <sched/shop/helper/JobShopTaskComparator.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>
#include <sched/types/EngineTraits.h>
#include <sched/types/ShopInstanceConcepts.h>

namespace sched::shop {

  template<typename Comparator>
  struct GeneralOperationListEngine {
    using Input = OperationListInput;
    using Schedule = JobShopTransportSchedule;

    template<typename Instance>
      requires(concepts::ShopTransportInstance<Instance>)
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const OperationListInput& input)
    {
      auto maybe_job_list = to_job_list(input, instance);

      if (!maybe_job_list) {
        return std::nullopt;
      }

      GeneralJobListEngine<Comparator> engine;
      return engine(instance, *maybe_job_list);
    }
  };

  using GeneralOperationListEngineEST = GeneralOperationListEngine<JobShopTaskEarliestStartingTime>;
  using GeneralOperationListEngineLST = GeneralOperationListEngine<JobShopTaskLatestStartingTime>;
  using GeneralOperationListEngineEFT = GeneralOperationListEngine<JobShopTaskEarliestFinishTime>;
  using GeneralOperationListEngineLFT = GeneralOperationListEngine<JobShopTaskLatestFinishTime>;
  using GeneralOperationListEngineSPT = GeneralOperationListEngine<JobShopTaskShortestProcessingTime>;
  using GeneralOperationListEngineLPT = GeneralOperationListEngine<JobShopTaskLargestProcessingTime>;

}

namespace sched {

  template<typename Comparator>
  struct EngineTraits<shop::GeneralOperationListEngine<Comparator>> {
    static std::string name()
    {
      using namespace std::literals;
      return "ope_"s + Comparator::name();
    }
  };

}

#endif // SCHED_SHOP_GENERAL_OPERATION_LIST_ENGINE_H
