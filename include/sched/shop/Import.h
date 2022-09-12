#ifndef SCHED_IMPORT_H
#define SCHED_IMPORT_H

#include <filesystem>

#include <sched/common/Api.h>

#include "JobShopInstance.h"
#include "FlexibleJobShopInstance.h"
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

  struct SCHED_API FlexibleJobShopBenchmark {
    std::string name;
    int jobs;
    int machines;
    Time optimum;
    Time upper_bound;
    Time lower_bound;
    std::filesystem::path path;
  };

  struct SCHED_API GeneralFlexibleJobShopBenchmark {
    std::string name;
    int jobs;
    int machines;
    int transportations;
    std::filesystem::path path;
  };

  struct SCHED_API Import {
    static std::vector<JobShopBenchmark> load_jssp_benchmarks(const std::filesystem::path& filename);
    static JobShopInstance load_jssp(const std::filesystem::path& filename);

    static std::vector<FlexibleJobShopBenchmark> load_fjssp_benchmarks(const std::filesystem::path& filename);
    static FlexibleJobShopInstance load_fjssp(const std::filesystem::path& filename);

    static std::vector<GeneralFlexibleJobShopBenchmark> load_gfjssp_benchmarks(const std::filesystem::path& filename);
    static GeneralFlexibleJobShopInstance load_gfjssp(const std::filesystem::path& filename);
  };

}



#endif // SCHED_IMPORT_H
