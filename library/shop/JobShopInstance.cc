#include <sched/shop/JobShopInstance.h>

#include <sched/shop/ShopInstanceConcepts.h>

static_assert(sched::concepts::ShopInstance<sched::shop::JobShopInstance>);
