// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/input/HoistInputConversion.h>

namespace sched::shop {

  HoistEmptyInput to_empty_input(const HoistLoadedInput& loaded_input, const HoistSchedulingInstance& instance)
  {
    return {};
  }

  HoistLoadedInput to_loaded_input(const HoistEmptyInput& empty_input, const HoistSchedulingInstance& instance)
  {
    return {};
  }

}
