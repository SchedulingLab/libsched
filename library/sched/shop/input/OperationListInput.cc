// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/input/OperationListInput.h>

#include <sched/types/InputConcepts.h>

namespace {

  struct DummyInstance { };

}

static_assert(sched::concepts::Input<sched::shop::OperationListInput>);
static_assert(sched::concepts::InputFor<sched::shop::OperationListInput, DummyInstance>);
