#include <sched/shop/engine/OperationListEngine.h>

#include <sched/types/EngineConcepts.h>
#include <sched/shop/instance/FlexibleJobShopInstance.h>
#include <sched/shop/instance/JobShopInstance.h>

static_assert(sched::concepts::EngineFor<sched::shop::OperationListEngine, sched::shop::JobShopInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::OperationListEngine, sched::shop::FlexibleJobShopInstance>);
