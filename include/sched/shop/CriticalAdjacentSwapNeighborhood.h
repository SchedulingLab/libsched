#ifndef SCHED_SHOP_CRITICAL_ADJACENT_SWAP_NEIGHBORHOOD_H
#define SCHED_SHOP_CRITICAL_ADJACENT_SWAP_NEIGHBORHOOD_H

#include <sched/common/Api.h>

#include "BasicNeighborhood.h"
#include "JobShopSchedule.h"
#include "MachineListInput.h"
#include "OperationListInput.h"

namespace sched::shop {

  struct SCHED_API OperationCriticalAdjacentSwapNeighborhood : BasicNeighborhood {

    OperationListInput operator()(const OperationListInput& input, const JobShopSchedule& schedule, Random& random);
    std::vector<OperationListInput> generate_many(const OperationListInput& input, const JobShopSchedule& schedule, Random& random, std::size_t count);
    bool are_neighbors(const OperationListInput& input0, const OperationListInput& input1);

  };


}


#endif // SCHED_SHOP_CRITICAL_ADJACENT_SWAP_NEIGHBORHOOD_H
