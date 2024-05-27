#include <sched/shop/GeneralMachineListEngine.h>

#include <sched/shop/EngineConcepts.h>
#include <sched/shop/FlexibleJobShopTransportInstance.h>

static_assert(sched::shop::EngineFor<sched::shop::GeneralMachineListEngineEST, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::shop::EngineFor<sched::shop::GeneralMachineListEngineLST, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::shop::EngineFor<sched::shop::GeneralMachineListEngineEFT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::shop::EngineFor<sched::shop::GeneralMachineListEngineLFT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::shop::EngineFor<sched::shop::GeneralMachineListEngineSPT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::shop::EngineFor<sched::shop::GeneralMachineListEngineLPT, sched::shop::FlexibleJobShopTransportInstance>);
