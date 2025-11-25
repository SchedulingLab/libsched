// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_HOIST_INPUT_CONVERSION_H
#define SCHED_SHOP_HOIST_INPUT_CONVERSION_H

#include <sched/Api.h>
#include <sched/shop/input/HoistEmptyInput.h>
#include <sched/shop/input/HoistLoadedInput.h>

namespace sched::shop {

  SCHED_API HoistEmptyInput to_empty_input(const HoistLoadedInput& loaded_input, std::size_t machine_count);
  SCHED_API HoistLoadedInput to_loaded_input(const HoistEmptyInput& empty_input, std::size_t machine_count);

}

#endif // SCHED_SHOP_HOIST_INPUT_CONVERSION_H
