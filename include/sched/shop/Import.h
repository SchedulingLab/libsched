#ifndef SCHED_IMPORT_H
#define SCHED_IMPORT_H

#include <filesystem>

#include <sched/common/Api.h>

#include "JobShopInstance.h"
#include "GeneralFlexibleJobShopInstance.h"

namespace sched::shop {

  enum class BenchmarkGenerator {
    Unknown,
    Const,
    Uniform99,
    Uniform200,
    Binomial,
    NegativeBinomial,
  };

  struct SCHED_API JobShopBenchmark {
    std::string name;
    int jobs;
    int machines;
    Time optimum;
    Time upper_bound;
    Time lower_bound;
    BenchmarkGenerator generator;
    std::filesystem::path path;
  };


  class SCHED_API Import {
  public:

    static std::vector<JobShopBenchmark> load_benchmarks(const std::filesystem::path& filename);

    static JobShopInstance load_job_shop(const std::filesystem::path& filename);

    static GeneralFlexibleJobShopInstance load_gfjssp(const std::filesystem::path& filename);

  };

}



#endif // SCHED_IMPORT_H
