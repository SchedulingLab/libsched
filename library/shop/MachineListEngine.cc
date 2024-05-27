#include <sched/shop/MachineListEngine.h>

#include <sched/shop/EngineConcepts.h>
#include <sched/shop/JobShopInstance.h>
#include <sched/shop/FlexibleJobShopInstance.h>

static_assert(sched::shop::EngineFor<sched::shop::MachineListEngine, sched::shop::JobShopInstance>);
static_assert(sched::shop::EngineFor<sched::shop::MachineListEngine, sched::shop::FlexibleJobShopInstance>);
