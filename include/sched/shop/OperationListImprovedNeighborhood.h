#ifndef SCHED_SHOP_OPERATION_LIST_IMPROVED_NEIGHBORHOOD_H
#define SCHED_SHOP_OPERATION_LIST_IMPROVED_NEIGHBORHOOD_H

#include <string>

#include <sched/common/Api.h>

#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"
#include "OperationListInput.h"

namespace sched::shop {

  struct SCHED_API OperationSwapNeighborhood {

    template<typename Schedule>
    OperationListInput operator()(const OperationListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random)
    {
      return compute(input, random);
    }

    template<typename Schedule>
    std::vector<OperationListInput> generate_many(const OperationListInput& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

  private:
    static OperationListInput compute(const OperationListInput& input, Random& random);
  };

  template<>
  struct NeighborhoodTraits<OperationSwapNeighborhood> {
    static std::string name()
    {
      return "swp";
    }
  };

  struct SCHED_API OperationInsertNeighborhood {

    template<typename Schedule>
    OperationListInput operator()(const OperationListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random)
    {
      return compute(input, random);
    }

    template<typename Schedule>
    std::vector<OperationListInput> generate_many(const OperationListInput& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

  private:
    static OperationListInput compute(const OperationListInput& input, Random& random);
  };

  template<>
  struct NeighborhoodTraits<OperationInsertNeighborhood> {
    static std::string name()
    {
      return "ins";
    }
  };

  struct SCHED_API OperationReverseNeighborhood {

    template<typename Schedule>
    OperationListInput operator()(const OperationListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random)
    {
      return compute(input, random);
    }

    template<typename Schedule>
    std::vector<OperationListInput> generate_many(const OperationListInput& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

  private:
    static OperationListInput compute(const OperationListInput& input, Random& random);
  };

  template<>
  struct NeighborhoodTraits<OperationReverseNeighborhood> {
    static std::string name()
    {
      return "rev";
    }
  };

  struct SCHED_API OperationCappedReverseNeighborhood {

    template<typename Schedule>
    OperationListInput operator()(const OperationListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random)
    {
      return compute(input, random);
    }

    template<typename Schedule>
    std::vector<OperationListInput> generate_many(const OperationListInput& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

  private:
    static OperationListInput compute(const OperationListInput& input, Random& random);
  };

  template<>
  struct NeighborhoodTraits<OperationCappedReverseNeighborhood> {
    static std::string name()
    {
      return "crv";
    }
  };

}

#endif // SCHED_SHOP_OPERATION_LIST_IMPROVED_NEIGHBORHOOD_H
