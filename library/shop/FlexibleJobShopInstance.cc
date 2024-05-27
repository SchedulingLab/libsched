#include <sched/shop/FlexibleJobShopInstance.h>

#include <sched/shop/ShopInstanceConcepts.h>

static_assert(sched::concepts::ShopInstance<sched::shop::FlexibleJobShopInstance>);
