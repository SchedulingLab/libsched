#ifndef SCHED_NEUTRALITY_RATE_ANALYSIS_H
#define SCHED_NEUTRALITY_RATE_ANALYSIS_H

#include <cassert>

#include <vector>

#include <sched/common/Api.h>

namespace sched::shop {

  struct SCHED_API NeutralityRateAnalysis {

    template<typename Fitness>
    double operator()(const std::vector<Fitness>& walk)
    {
      assert(walk.size() > 0);
      std::size_t count = 0;
      std::size_t change = 0;

      std::size_t same = 0;
      std::size_t local_count = 0;

      Fitness previous;

      for (auto fitness : walk) {
        if (count == 0) {
          previous = fitness;
        } else {
          if (fitness != previous) {
            ++change;
            previous = fitness;

            same += local_count;
            local_count = 0;
          } else {
            ++local_count;
          }
        }

        ++count;
      }

      same += local_count;

      assert(count == walk.size());
      assert(same == count - 1 - change);

      return 100.0 * static_cast<double>(count - 1 - change) / static_cast<double>(count - 1);
    }
  };

}

#endif // SCHED_NEUTRALITY_RATE_ANALYSIS_H
