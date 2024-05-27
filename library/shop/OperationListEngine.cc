#include <sched/shop/OperationListEngine.h>

#include <sched/shop/EngineConcepts.h>
#include <sched/shop/FlexibleJobShopInstance.h>
#include <sched/shop/JobShopInstance.h>

static_assert(sched::concepts::EngineFor<sched::shop::OperationListEngine, sched::shop::JobShopInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::OperationListEngine, sched::shop::FlexibleJobShopInstance>);
