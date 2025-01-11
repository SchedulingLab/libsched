// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/engine/TransportationListEngine.h>

#include <sched/types/EngineConcepts.h>
#include <sched/shop/instance/FlexibleJobShopTransportInstance.h>

static_assert(sched::concepts::EngineFor<sched::shop::TransportationListEngineEST, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::TransportationListEngineLST, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::TransportationListEngineEFT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::TransportationListEngineLFT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::TransportationListEngineSPT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::TransportationListEngineLPT, sched::shop::FlexibleJobShopTransportInstance>);
