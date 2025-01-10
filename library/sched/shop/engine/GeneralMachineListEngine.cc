#include <sched/shop/engine/GeneralMachineListEngine.h>

#include <sched/types/EngineConcepts.h>
#include <sched/shop/instance//FlexibleJobShopTransportInstance.h>

static_assert(sched::concepts::EngineFor<sched::shop::GeneralMachineListEngineEST, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::GeneralMachineListEngineLST, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::GeneralMachineListEngineEFT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::GeneralMachineListEngineLFT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::GeneralMachineListEngineSPT, sched::shop::FlexibleJobShopTransportInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::GeneralMachineListEngineLPT, sched::shop::FlexibleJobShopTransportInstance>);
