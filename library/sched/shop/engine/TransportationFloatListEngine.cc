// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/engine/TransportationFloatListEngine.h>

#include <sched/types/EngineConcepts.h>
#include <sched/shop/instance/FlexibleJobShopTransportInstance.h>

static_assert(sched::concepts::EngineFor<sched::shop::TransportationFloatListEngineEST, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::TransportationFloatListEngineLST, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::TransportationFloatListEngineEFT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::TransportationFloatListEngineLFT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::TransportationFloatListEngineSPT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::TransportationFloatListEngineLPT, sched::shop::FlexibleJobShopTransportInstance>);
