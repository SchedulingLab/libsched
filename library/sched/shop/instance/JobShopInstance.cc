#include <sched/shop/instance/JobShopInstance.h>

#include <sched/types/ShopInstanceConcepts.h>

static_assert(sched::concepts::ShopInstance<sched::shop::JobShopInstance>);
