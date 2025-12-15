// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/input/InputFmt.h>

static_assert(std::formattable<sched::shop::JobListInput, char>);
static_assert(std::formattable<sched::shop::OperationListInput, char>);
static_assert(std::formattable<sched::shop::MachineListInput, char>);
