#include <sched/shop/Import.h>

#include <cassert>
#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

namespace sched::shop {

  /*
   * jsp
   */

  NLOHMANN_JSON_SERIALIZE_ENUM(BenchmarkGenerator, {
    { BenchmarkGenerator::Const, "gen-const" },
    { BenchmarkGenerator::Uniform99, "gen-uniform-99" },
    { BenchmarkGenerator::Uniform200, "gen-uniform-200" },
    { BenchmarkGenerator::Binomial, "gen-binom" },
    { BenchmarkGenerator::NegativeBinomial, "gen-nbinom" },
  })


  static void from_json(const nlohmann::json& j, JobShopBenchmark& benchmark) {
    assert(!j.is_null());
    j.at("name").get_to(benchmark.name);
    j.at("jobs").get_to(benchmark.jobs);
    j.at("machines").get_to(benchmark.machines);

    if (j.at("optimum").is_null()) {
      benchmark.optimum = 0;
      auto & bounds = j.at("bounds");

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

  std::vector<JobShopBenchmark> Import::load_jsp_benchmarks(const std::filesystem::path& filename) {
    std::vector<JobShopBenchmark> benchmarks;

    std::ifstream stream(filename);
    nlohmann::json root;
    stream >> root;

    assert(root.is_array());
    root.get_to(benchmarks);
    return benchmarks;
  }

  JobShopInstance Import::load_jsp(const std::filesystem::path& filename) {
    std::ifstream input(filename);

    if (!input) {
      throw std::runtime_error("File not found: " + filename.string());
    }

    std::size_t machine_count;
    std::size_t job_count;

    for (std::string line; std::getline(input, line); ) {
      if (line[0] == '#') {
        continue;
      }

      std::istringstream first;
      first.str(line);
      first >> job_count >> machine_count;
      break;
    }

    std::vector<JobShopInstance::JobDesc> jobs;

    for (std::string line; std::getline(input, line); ) {
      std::istringstream data;
      data.str(line);

      JobShopInstance::JobDesc job;
      std::size_t machine;
      Time processing;

      while (data >> machine >> processing) {
        JobShopInstance::OperationDesc op = { MachineId{machine}, processing };
        job.operations.push_back(op);
      }

//       assert(job.operations.size() == machine_count);
      jobs.push_back(std::move(job));
    }

    assert(jobs.size() == job_count);

    return JobShopInstance(machine_count, std::move(jobs));
  }

  /*
   * fjsp
   */

  static void from_json(const nlohmann::json& j, FlexibleJobShopBenchmark& benchmark) {
    assert(!j.is_null());
    j.at("name").get_to(benchmark.name);
    j.at("jobs").get_to(benchmark.jobs);
    j.at("machines").get_to(benchmark.machines);

    if (j.at("optimum").is_null()) {
      benchmark.optimum = 0;
      auto & bounds = j.at("bounds");

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

  std::vector<FlexibleJobShopBenchmark> Import::load_fjsp_benchmarks(const std::filesystem::path& filename) {
    std::vector<FlexibleJobShopBenchmark> benchmarks;

    std::ifstream stream(filename);
    nlohmann::json root;
    stream >> root;

    assert(root.is_array());
    root.get_to(benchmarks);
    return benchmarks;
  }

  FlexibleJobShopInstance Import::load_fjsp(const std::filesystem::path& filename) {
    std::ifstream input(filename);

    if (!input) {
      throw std::runtime_error("File not found: " + filename.string());
    }

    std::size_t machine_count;
    std::size_t job_count;

    for (std::string line; std::getline(input, line); ) {
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

      std::size_t operation_count;
      data >> operation_count;
      assert(operation_count > 0);

      FlexibleJobShopInstance::JobDesc job;
      std::size_t operation_machine_count;

      while (data >> operation_machine_count) {
        FlexibleJobShopInstance::OperationDesc operation;
        std::size_t machine;
        Time processing;

        for (std::size_t j = 0; j < operation_machine_count; ++j) {
          data >> machine >> processing;
          operation.choices.push_back({ MachineId{machine}, processing });
        }

        job.operations.push_back(std::move(operation));
      }

      assert(job.operations.size() == operation_count);
      jobs.push_back(std::move(job));
    }

    return FlexibleJobShopInstance(machine_count, std::move(jobs));
  }

  /*
   * fjspt
   */

  static void from_json(const nlohmann::json& j, FlexibleJobShopTransportBenchmark& benchmark) {
    assert(!j.is_null());
    j.at("name").get_to(benchmark.name);
    j.at("jobs").get_to(benchmark.jobs);
    j.at("machines").get_to(benchmark.machines);
    j.at("transportations").get_to(benchmark.transportations);

    std::string path_string;
    j.at("path").get_to(path_string);
    benchmark.path = path_string;
  }

  std::vector<FlexibleJobShopTransportBenchmark> Import::load_fjspt_benchmarks(const std::filesystem::path& filename) {
    std::vector<FlexibleJobShopTransportBenchmark> benchmarks;

    std::ifstream stream(filename);
    nlohmann::json root;
    stream >> root;

    assert(root.is_array());
    root.get_to(benchmarks);
    return benchmarks;
  }

  FlexibleJobShopTransportInstance Import::load_fjspt(const std::filesystem::path& filename) {
    std::ifstream input(filename);

    if (!input) {
      throw std::runtime_error("File not found: " + filename.string());
    }

    std::size_t machine_count;
    std::size_t job_count;

    for (std::string line; std::getline(input, line); ) {
      if (line[0] == '#') {
        continue;
      }

      std::istringstream first;
      first.str(line);
      first >> job_count >> machine_count;
      break;
    }

    std::vector<FlexibleJobShopTransportInstance::JobDesc> jobs;

    for (std::size_t i = 0; i < job_count; ++i) {
      std::string line;

      if (!std::getline(input, line)) {
        throw std::runtime_error("Missing data for job #" + std::to_string(i));
      }

      std::istringstream data;
      data.str(line);

      FlexibleJobShopTransportInstance::JobDesc job;
      std::size_t operation_machine_count;

      while (data >> operation_machine_count) {
        FlexibleJobShopTransportInstance::FlexibleOperationDesc operation;
        std::size_t machine;
        Time processing;

        for (std::size_t j = 0; j < operation_machine_count; ++j) {
          data >> machine >> processing;
          operation.choices.push_back({ MachineId{machine}, processing });
        }

        job.operations.push_back(std::move(operation));
      }

      jobs.push_back(std::move(job));
    }

    std::size_t transportation_resources;

    input >> transportation_resources;

    Array2D<Time> delays(machine_count, machine_count);
    std::size_t from = 0;

    for (std::string line; std::getline(input, line); ) {
      std::istringstream data;
      data.str(line);

      Time duration;
      std::size_t to = 0;

      while (data >> duration) {
        delays(from, to) = duration;
        ++to;
      }

      assert(to == machine_count);
      ++from;
    }

    assert(from == machine_count);
    return FlexibleJobShopTransportInstance(machine_count, std::move(jobs), transportation_resources, delays, delays);
  }

}

