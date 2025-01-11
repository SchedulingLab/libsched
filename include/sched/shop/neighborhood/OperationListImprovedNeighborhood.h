// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_OPERATION_LIST_IMPROVED_NEIGHBORHOOD_H
#define SCHED_SHOP_OPERATION_LIST_IMPROVED_NEIGHBORHOOD_H

#include <string>

#include <sched/Api.h>
#include <sched/meta/neighborhood/NeighborhoodHelper.h>
#include <sched/types/NeighborhoodTraits.h>
#include <sched/shop/input/OperationListInput.h>

namespace sched {

  namespace shop {

    struct SCHED_API OperationSwapNeighborhood {

      template<typename Schedule>
      OperationListInput operator()(const OperationListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random)
      {
        return compute(input, random);
      }

      template<typename Schedule>
      std::vector<OperationListInput> generate_many(const OperationListInput& input, const Schedule& schedule, Random& random, std::size_t count)
      {
        return generate_many_neighbors(*this, input, schedule, random, count);
      }

    private:
      static OperationListInput compute(const OperationListInput& input, Random& random);
    };

  }

  template<>
  struct NeighborhoodTraits<shop::OperationSwapNeighborhood> {
    static std::string name()
    {
      return "swp";
    }
  };

  namespace shop {

    struct SCHED_API OperationInsertNeighborhood {

      template<typename Schedule>
      OperationListInput operator()(const OperationListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random)
      {
        return compute(input, random);
      }

      template<typename Schedule>
      std::vector<OperationListInput> generate_many(const OperationListInput& input, const Schedule& schedule, Random& random, std::size_t count)
      {
        return generate_many_neighbors(*this, input, schedule, random, count);
      }

    private:
      static OperationListInput compute(const OperationListInput& input, Random& random);
    };

  }

  template<>
  struct NeighborhoodTraits<shop::OperationInsertNeighborhood> {
    static std::string name()
    {
      return "ins";
    }
  };

  namespace shop {

    struct SCHED_API OperationReverseNeighborhood {

      template<typename Schedule>
      OperationListInput operator()(const OperationListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random)
      {
        return compute(input, random);
      }

      template<typename Schedule>
      std::vector<OperationListInput> generate_many(const OperationListInput& input, const Schedule& schedule, Random& random, std::size_t count)
      {
        return generate_many_neighbors(*this, input, schedule, random, count);
      }

    private:
      static OperationListInput compute(const OperationListInput& input, Random& random);
    };

  }

  template<>
  struct NeighborhoodTraits<shop::OperationReverseNeighborhood> {
    static std::string name()
    {
      return "rev";
    }
  };

  namespace shop {

    struct SCHED_API OperationCappedReverseNeighborhood {

      template<typename Schedule>
      OperationListInput operator()(const OperationListInput& input, [[maybe_unused]] const Schedule& schedule, Random& random)
      {
        return compute(input, random);
      }

      template<typename Schedule>
      std::vector<OperationListInput> generate_many(const OperationListInput& input, const Schedule& schedule, Random& random, std::size_t count)
      {
        return generate_many_neighbors(*this, input, schedule, random, count);
      }

    private:
      static OperationListInput compute(const OperationListInput& input, Random& random);
    };

  }

  template<>
  struct NeighborhoodTraits<shop::OperationCappedReverseNeighborhood> {
    static std::string name()
    {
      return "crv";
    }
  };

}

#endif // SCHED_SHOP_OPERATION_LIST_IMPROVED_NEIGHBORHOOD_H
