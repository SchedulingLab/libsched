// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/engine/MachineListEngine.h>

#include <sched/types/EngineConcepts.h>
#include <sched/shop/instance/JobShopInstance.h>
#include <sched/shop/instance/FlexibleJobShopInstance.h>

static_assert(sched::concepts::EngineFor<sched::shop::MachineListEngine, sched::shop::JobShopInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::MachineListEngine, sched::shop::FlexibleJobShopInstance>);
