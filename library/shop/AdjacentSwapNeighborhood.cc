#include <sched/shop/AdjacentSwapNeighborhood.h>

#include <sched/shop/NeighborhoodConcepts.h>
#include <sched/shop/JobListInput.h>

static_assert(sched::concepts::NeighborhoodFor<sched::shop::AdjacentSwapNeighborhood, sched::shop::JobListInput>);
