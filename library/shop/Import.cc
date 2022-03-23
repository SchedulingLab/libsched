#include <sched/shop/Import.h>

#include <cassert>
#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

namespace sched::shop {

  NLOHMANN_JSON_SERIALIZE_ENUM(BenchmarkGenerator, {
    { BenchmarkGenerator::Const, "gen-const" },
    { BenchmarkGenerator::Uniform99, "gen-uniform-99" },
    { BenchmarkGenerator::Uniform200, "gen-uniform-200" },
    { BenchmarkGenerator::Binomial, "gen-binom" },
    { BenchmarkGenerator::NegativeBinomial, "gen-nbinom" },
  })


  void from_json(const nlohmann::json& j, BenchmarkInstance& instance) {
    assert(!j.is_null());
    j.at("name").get_to(instance.name);
    j.at("jobs").get_to(instance.jobs);
    j.at("machines").get_to(instance.machines);

    if (j.at("optimum").is_null()) {
      instance.optimum = 0;
      auto & bounds = j.at("bounds");

      if (bounds.is_null()) {
        instance.upper_bound = 0;
        instance.lower_bound = 0;
      } else {
        j.at("bounds").at("upper").get_to(instance.upper_bound);
        j.at("bounds").at("lower").get_to(instance.lower_bound);
      }
    } else {
      j.at("optimum").get_to(instance.optimum);
      instance.upper_bound = instance.optimum;
      instance.lower_bound = instance.optimum;
    }

    if (j.contains("generator")) {
      j.at("generator").get_to(instance.generator);
    } else {
      instance.generator = BenchmarkGenerator::Unknown;
    }

    std::string path_string;
    j.at("path").get_to(path_string);
    instance.path = path_string;
  }


  std::vector<BenchmarkInstance> Import::load_benchmarks(const std::filesystem::path& filename) {
    std::vector<BenchmarkInstance> instances;

    std::ifstream stream(filename);
    nlohmann::json root;
    stream >> root;

    assert(root.is_array());
    root.get_to(instances);
    return instances;
  }

  JobShopInstance Import::load_job_shop(const std::filesystem::path& filename) {
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

}

