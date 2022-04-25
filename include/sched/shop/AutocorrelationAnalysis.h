#ifndef SCHED_AUTOCORRELATION_ANALYSIS_H
#define SCHED_AUTOCORRELATION_ANALYSIS_H

#include <cmath>

#include <sched/common/Api.h>
#include <sched/tools/Statistics.h>

#include <vector>

namespace sched::shop {

  struct SCHED_API AutocorrelationAnalysis {

    template<typename Fitness>
    std::vector<double> operator()(const std::vector<Fitness>& walk, std::size_t count) {
      std::vector<double> results;

      for (std::size_t offset = 1; offset <= count; ++offset) {
        std::size_t count = walk.size() - offset;

        Statistics ft0;
        Statistics ftl;
        Statistics ft0_x_ftl;

        for (std::size_t i = 0; i < count; ++i) {
          double x0 = walk[i];
          double xl = walk[i + offset];
          ft0.add(x0);
          ftl.add(xl);
          ft0_x_ftl.add(x0 * xl);
        }

        double var0 = ft0.get_variance();

        if (var0 < std::numeric_limits<double>::epsilon()) {
          // all the values are the same
          results.push_back(1.0);
        } else {
          results.push_back(std::abs(ft0_x_ftl.get_mean() - ft0.get_mean() * ftl.get_mean()) / var0);
        }
      }

      return results;
    }
  };

}

#endif // SCHED_AUTOCORRELATION_ANALYSIS_H
