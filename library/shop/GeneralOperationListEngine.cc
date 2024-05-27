#include <sched/shop/GeneralOperationListEngine.h>

#include <sched/shop/EngineConcepts.h>
#include <sched/shop/FlexibleJobShopTransportInstance.h>

static_assert(sched::concepts::EngineFor<sched::shop::GeneralOperationListEngineEST, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::GeneralOperationListEngineLST, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::GeneralOperationListEngineEFT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::GeneralOperationListEngineLFT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::GeneralOperationListEngineSPT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::GeneralOperationListEngineLPT, sched::shop::FlexibleJobShopTransportInstance>);
