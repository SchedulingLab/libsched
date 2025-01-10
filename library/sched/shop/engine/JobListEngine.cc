#include <sched/shop/engine/JobListEngine.h>

#include <sched/types/EngineConcepts.h>
#include <sched/shop/instance/FlexibleJobShopInstance.h>
#include <sched/shop/instance/JobShopInstance.h>

static_assert(sched::concepts::EngineFor<sched::shop::JobListEngine, sched::shop::JobShopInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::JobListEngine, sched::shop::FlexibleJobShopInstance>);
