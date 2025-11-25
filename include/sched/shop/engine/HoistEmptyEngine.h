// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_HOIST_EMPTY_ENGINE_H
#define SCHED_SHOP_HOIST_EMPTY_ENGINE_H

#include <cassert>

#include <optional>

#include <sched/Api.h>
#include <sched/shop/engine/HoistLoadedEngine.h>
#include <sched/shop/input/HoistEmptyInput.h>
#include <sched/shop/input/HoistInputConversion.h>
#include <sched/shop/schedule/HoistSchedule.h>
#include <sched/types/EngineTraits.h>
#include <sched/types/ShopInstanceConcepts.h>

namespace sched::shop {

  struct SCHED_API HoistEmptyEngine {
    using Input = HoistEmptyInput;
    using Schedule = HoistSchedule;

    template<typename Instance>
      // requires(concepts::ShopInstance<Instance>)
    std::optional<HoistSchedule> operator()(const Instance& instance, const HoistEmptyInput& input)
    {
      HoistLoadedEngine engine;
      return engine(instance, to_loaded_input(input, instance.machine_count()));
    }
  };

}

namespace sched {

  template<>
  struct EngineTraits<shop::HoistEmptyEngine> {
    static std::string name()
    {
      using namespace std::literals;
      return "mpt";
    }
  };

}

#endif // SCHED_SHOP_HOIST_EMPTY_ENGINE_H
