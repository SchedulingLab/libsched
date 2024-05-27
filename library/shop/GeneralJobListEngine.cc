#include <sched/shop/GeneralJobListEngine.h>

#include <sched/shop/EngineConcepts.h>
#include <sched/shop/FlexibleJobShopTransportInstance.h>

static_assert(sched::shop::EngineFor<sched::shop::GeneralJobListEngineEST, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::shop::EngineFor<sched::shop::GeneralJobListEngineLST, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::shop::EngineFor<sched::shop::GeneralJobListEngineEFT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::shop::EngineFor<sched::shop::GeneralJobListEngineLFT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::shop::EngineFor<sched::shop::GeneralJobListEngineSPT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::shop::EngineFor<sched::shop::GeneralJobListEngineLPT, sched::shop::FlexibleJobShopTransportInstance>);
