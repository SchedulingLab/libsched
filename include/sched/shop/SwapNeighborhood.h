#ifndef SCHED_SWAP_NEIHGBORHOOD_H
#define SCHED_SWAP_NEIHGBORHOOD_H

#include "MutationNeighborhood.h"
#include "SwapMutation.h"

namespace sched::shop {

  using SwapNeighborhood = MutationNeighborhood<SwapMutation>;

}

#endif // SCHED_SWAP_NEIHGBORHOOD_H
