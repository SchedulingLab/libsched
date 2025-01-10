#include <sched/meta/neighborhood/AdjacentSwapNeighborhood.h>

#include <sched/types/NeighborhoodConcepts.h>
#include <sched/shop/input/JobListInput.h>

static_assert(sched::concepts::NeighborhoodFor<sched::AdjacentSwapNeighborhood, sched::shop::JobListInput>);
