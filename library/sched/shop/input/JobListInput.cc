#include <sched/shop/input/JobListInput.h>

#include <sched/types/InputConcepts.h>

static_assert(sched::concepts::Input<sched::shop::JobListInput>);
