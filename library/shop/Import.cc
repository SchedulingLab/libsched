// clang-format off: main header
#include <sched/shop/Import.h>
// clang-format on

#include <cassert>

#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

#include <sched/tools/Log.h>

namespace sched::shop {

  namespace {
    struct InputFile {
      InputFile(const std::filesystem::path& filename)
      : input(filename)
      {
        if (!input) {
          throw std::runtime_error("File not found: " + filename.string());
        }

        for (std::string line; std::getline(input, line);) {
          if (line[0] == '#') {
            continue;
          }

          int64_t value = 0;
          std::istringstream line_input(line);

          while (line_input >> value) {
            data.push_back(value);
          }
        }

        iterator = data.cbegin();
      }

      int64_t next()
      {
        if (iterator != data.cend()) {
          return *iterator++;
        }

        throw std::runtime_error("Bad format!");
      }

      bool finished() const
      {
        return iterator == data.cend();
      }

      std::ifstream input;
      std::vector<int64_t> data;
      std::vector<int64_t>::const_iterator iterator;
    };

  }

  /*
   * jsp
   */

  // clang-format off
  NLOHMANN_JSON_SERIALIZE_ENUM(BenchmarkGenerator, {
    {            BenchmarkGenerator::Const,       "gen-const" },
    {        BenchmarkGenerator::Uniform99,  "gen-uniform-99" },
    {       BenchmarkGenerator::Uniform200, "gen-uniform-200" },
    {         BenchmarkGenerator::Binomial,       "gen-binom" },
    { BenchmarkGenerator::NegativeBinomial,      "gen-nbinom" },
  })
  // clang-format on

  void from_json(const nlohmann::json& j, JobShopBenchmark& benchmark)
  {
    assert(!j.is_null());
    j.at("name").get_to(benchmark.name);
    j.at("jobs").get_to(benchmark.jobs);
    j.at("machines").get_to(benchmark.machines);

    if (j.at("optimum").is_null()) {
      benchmark.optimum = 0;
      const auto& bounds = j.at("bounds");

      if (bounds.is_null()) {
        benchmark.upper_bound = 0;
        benchmark.lower_bound = 0;
      } else {
        j.at("bounds").at("upper").get_to(benchmark.upper_bound);
        j.at("bounds").at("lower").get_to(benchmark.lower_bound);
      }
    } else {
      j.at("optimum").get_to(benchmark.optimum);
      benchmark.upper_bound = benchmark.optimum;
      benchmark.lower_bound = benchmark.optimum;
    }

    if (j.contains("generator")) {
      j.at("generator").get_to(benchmark.generator);
    } else {
      benchmark.generator = BenchmarkGenerator::Unknown;
    }

    std::string path_string;
    j.at("path").get_to(path_string);
    benchmark.path = path_string;
  }

  std::vector<JobShopBenchmark> Import::load_jsp_benchmarks(const std::filesystem::path& filename)
  {
    std::vector<JobShopBenchmark> benchmarks;

    std::ifstream stream(filename);
    nlohmann::json root;
    stream >> root;

    assert(root.is_array());
    root.get_to(benchmarks);
    return benchmarks;
  }

  JobShopInstance Import::load_jsp(const std::filesystem::path& filename)
  {
    std::ifstream input(filename);

    if (!input) {
      throw std::runtime_error("File not found: " + filename.string());
    }

    std::size_t machine_count = 0;
    std::size_t job_count = 0;

    for (std::string line; std::getline(input, line);) {
      if (line[0] == '#') {
        continue;
      }

      std::istringstream first;
      first.str(line);
      first >> job_count >> machine_count;
      break;
    }

    std::vector<JobShopInstance::JobDesc> jobs;

    for (std::string line; std::getline(input, line);) {
      std::istringstream data;
      data.str(line);

      JobShopInstance::JobDesc job;
      std::size_t machine = 0;
      Time processing = 0;

      while (data >> machine >> processing) {
        JobShopInstance::OperationDesc op = { MachineId{ machine }, processing };
        job.operations.push_back(op);
      }

      jobs.push_back(std::move(job));
    }

    assert(jobs.size() == job_count);

    return { machine_count, std::move(jobs) };
  }

  /*
   * fjsp
   */

  void from_json(const nlohmann::json& j, FlexibleJobShopBenchmark& benchmark)
  {
    assert(!j.is_null());
    j.at("name").get_to(benchmark.name);
    j.at("jobs").get_to(benchmark.jobs);
    j.at("machines").get_to(benchmark.machines);

    if (j.at("optimum").is_null()) {
      benchmark.optimum = 0;
      const auto& bounds = j.at("bounds");

      if (bounds.is_null()) {
        benchmark.upper_bound = 0;
        benchmark.lower_bound = 0;
      } else {
        j.at("bounds").at("upper").get_to(benchmark.upper_bound);
        j.at("bounds").at("lower").get_to(benchmark.lower_bound);
      }
    } else {
      j.at("optimum").get_to(benchmark.optimum);
      benchmark.upper_bound = benchmark.optimum;
      benchmark.lower_bound = benchmark.optimum;
    }

    std::string path_string;
    j.at("path").get_to(path_string);
    benchmark.path = path_string;
  }

  std::vector<FlexibleJobShopBenchmark> Import::load_fjsp_benchmarks(const std::filesystem::path& filename)
  {
    std::vector<FlexibleJobShopBenchmark> benchmarks;

    std::ifstream stream(filename);
    nlohmann::json root;
    stream >> root;

    assert(root.is_array());
    root.get_to(benchmarks);
    return benchmarks;
  }

  FlexibleJobShopInstance Import::load_fjsp(const std::filesystem::path& filename)
  {
    std::ifstream input(filename);

    if (!input) {
      throw std::runtime_error("File not found: " + filename.string());
    }

    std::size_t machine_count = 0;
    std::size_t job_count = 0;

    for (std::string line; std::getline(input, line);) {
      if (line[0] == '#') {
        continue;
      }

      std::istringstream first;
      first.str(line);
      first >> job_count >> machine_count;
      break;
    }

    std::vector<FlexibleJobShopInstance::JobDesc> jobs;

    for (std::size_t i = 0; i < job_count; ++i) {
      std::string line;

      if (!std::getline(input, line)) {
        throw std::runtime_error("Missing data for job #" + std::to_string(i));
      }

      std::istringstream data;
      data.str(line);

      std::size_t operation_count = 0;
      data >> operation_count;
      assert(operation_count > 0);

      FlexibleJobShopInstance::JobDesc job;
      std::size_t operation_machine_count = 0;

      while (data >> operation_machine_count) {
        FlexibleJobShopInstance::OperationDesc operation;
        std::size_t machine = 0;
        Time processing = 0;

        for (std::size_t j = 0; j < operation_machine_count; ++j) {
          data >> machine >> processing;
          operation.choices.push_back({ MachineId{ machine }, processing });
        }

        job.operations.push_back(std::move(operation));
      }

      assert(job.operations.size() == operation_count);
      jobs.push_back(std::move(job));
    }

    return { machine_count, std::move(jobs) };
  }

  /*
   * fjspt
   */

  void from_json(const nlohmann::json& j, FlexibleJobShopTransportBenchmark& benchmark)
  {
    assert(!j.is_null());
    j.at("name").get_to(benchmark.name);
    j.at("jobs").get_to(benchmark.jobs);
    j.at("machines").get_to(benchmark.machines);
    j.at("transportations").get_to(benchmark.transportations);

    std::string path_string;
    j.at("path").get_to(path_string);
    benchmark.path = path_string;
  }

  std::vector<FlexibleJobShopTransportBenchmark> Import::load_fjspt_benchmarks(const std::filesystem::path& filename)
  {
    std::vector<FlexibleJobShopTransportBenchmark> benchmarks;

    std::ifstream stream(filename);
    nlohmann::json root;
    stream >> root;

    assert(root.is_array());
    root.get_to(benchmarks);
    return benchmarks;
  }

  FlexibleJobShopTransportInstance Import::load_fjspt(const std::filesystem::path& filename)
  {
    InputFile input(filename);

    const std::size_t job_count = input.next();
    const std::size_t machine_count = input.next();

    // Log::println("machine_count: {}, job_count: {}", machine_count, job_count);

    std::vector<FlexibleJobShopTransportInstance::JobDesc> jobs;

    for (std::size_t i = 0; i < job_count; ++i) {
      const std::size_t operation_count = input.next();
      // Log::println("[{}] operation_count: {}", i, operation_count);

      FlexibleJobShopTransportInstance::JobDesc job;

      for (std::size_t j = 0; j < operation_count; ++j) {
        const std::size_t operation_machine_count = input.next();
        // Log::println("[{}, {}] operation_machine_count: {}", i, j, operation_machine_count);

        FlexibleJobShopTransportInstance::FlexibleOperationDesc operation;

        for (std::size_t k = 0; k < operation_machine_count; ++k) {
          const std::size_t machine = input.next();
          const Time processing = input.next();
          // Log::println("[{}, {}, {}] machine: {}, processing: {}", i, j, k, machine, processing);
          operation.choices.push_back({ MachineId{ machine }, processing });
        }

        job.operations.push_back(std::move(operation));
      }

      jobs.push_back(std::move(job));
    }

    const std::size_t transportation_resources = input.next();

    // Log::println("transportation_resources: {}", transportation_resources);

    Array2D<Time> delays(machine_count, machine_count);

    for (std::size_t from = 0; from < machine_count; ++from) {
      for (std::size_t to = 0; to < machine_count; ++to) {
        delays(from, to) = input.next();
      }
    }

    assert(input.finished());
    return { machine_count, std::move(jobs), transportation_resources, delays, delays };
  }

}
