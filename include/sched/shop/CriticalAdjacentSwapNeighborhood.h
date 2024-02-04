#ifndef SCHED_SHOP_CRITICAL_ADJACENT_SWAP_NEIGHBORHOOD_H
#define SCHED_SHOP_CRITICAL_ADJACENT_SWAP_NEIGHBORHOOD_H

#include <string>

#include <sched/common/Api.h>

#include "JobShopSchedule.h"
#include "MachineListInput.h"
#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"
#include "OperationListInput.h"

namespace sched::shop {

  struct SCHED_API OperationCriticalAdjacentSwapNeighborhood {

    OperationListInput operator()(const OperationListInput& input, const JobShopSchedule& schedule, Random& random);
    std::vector<OperationListInput> generate_many(const OperationListInput& input, const JobShopSchedule& schedule, Random& random, std::size_t count);
  };

  template<>
  struct NeighborhoodTraits<OperationCriticalAdjacentSwapNeighborhood> {
    static std::string name()
    {
      return "cas";
    }
  };

  struct SCHED_API MachineCriticalAdjacentSwapNeighborhood {

    MachineListInput operator()(const MachineListInput& input, const JobShopSchedule& schedule, Random& random);
    std::vector<MachineListInput> generate_many(const MachineListInput& input, const JobShopSchedule& schedule, Random& random, std::size_t count);
  };

  template<>
  struct NeighborhoodTraits<MachineCriticalAdjacentSwapNeighborhood> {
    static std::string name()
    {
      return "cas";
    }
  };

}

#endif // SCHED_SHOP_CRITICAL_ADJACENT_SWAP_NEIGHBORHOOD_H
