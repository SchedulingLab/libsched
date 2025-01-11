// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/engine/GeneralOperationListEngine.h>

#include <sched/types/EngineConcepts.h>
#include <sched/shop/instance/FlexibleJobShopTransportInstance.h>

static_assert(sched::concepts::EngineFor<sched::shop::GeneralOperationListEngineEST, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::GeneralOperationListEngineLST, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::GeneralOperationListEngineEFT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::GeneralOperationListEngineLFT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::GeneralOperationListEngineSPT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::GeneralOperationListEngineLPT, sched::shop::FlexibleJobShopTransportInstance>);
