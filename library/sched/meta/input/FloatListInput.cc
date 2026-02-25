// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/meta/input/FloatListInput.h>

#include <sched/types/InputConcepts.h>

namespace {

  struct DummyInstance { };

}

static_assert(sched::concepts::Input<sched::FloatListInput>);
static_assert(sched::concepts::InputFor<sched::FloatListInput, DummyInstance>);
