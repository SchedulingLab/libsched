// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/input/HoistLoadedInput.h>

#include <sched/types/InputConcepts.h>

static_assert(sched::concepts::Input<sched::shop::HoistLoadedInput>);
