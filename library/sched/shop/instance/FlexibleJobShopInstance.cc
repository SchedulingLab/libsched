#include <sched/shop/instance/FlexibleJobShopInstance.h>

#include <sched/types/ShopInstanceConcepts.h>

static_assert(sched::concepts::ShopInstance<sched::shop::FlexibleJobShopInstance>);
