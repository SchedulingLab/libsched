// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/input/MachineListInput.h>

#include <sched/types/InputConcepts.h>

namespace {

  struct DummyInstance { };

}

static_assert(sched::concepts::InputArray<sched::shop::MachineListInput>);
static_assert(sched::concepts::InputFor<sched::shop::MachineListInput, DummyInstance>);
