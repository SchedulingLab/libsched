// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/input/HoistInputFmt.h>

#include <format>

#include <sched/shop/input/HoistLoadedInput.h>

static_assert(std::formattable<sched::shop::HoistLoadedInput, char>);
static_assert(std::formattable<sched::shop::HoistEmptyInput, char>);
