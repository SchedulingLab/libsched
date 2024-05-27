#include <sched/shop/JobShopInstance.h>

#include <sched/shop/ShopInstanceConcepts.h>

static_assert(sched::shop::ShopInstance<sched::shop::JobShopInstance>);
