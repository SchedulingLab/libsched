#include <sched/shop/JobListEngine.h>

#include <sched/shop/EngineConcepts.h>
#include <sched/shop/FlexibleJobShopInstance.h>
#include <sched/shop/JobShopInstance.h>

static_assert(sched::shop::EngineFor<sched::shop::JobListEngine, sched::shop::JobShopInstance>);
static_assert(sched::shop::EngineFor<sched::shop::JobListEngine, sched::shop::FlexibleJobShopInstance>);
