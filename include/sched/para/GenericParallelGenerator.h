#ifndef SCHED_PARA_GENERIC_PARALLEL_GENERATOR_H
#define SCHED_PARA_GENERIC_PARALLEL_GENERATOR_H

#include <random>

#include <sched/common/Api.h>
#include <sched/common/Random.h>
#include <sched/common/Time.h>

#include "ParallelInstance.h"

namespace sched::para {

  template<typename Distribution>
  struct GenericParallelGenerator {
    GenericParallelGenerator(std::size_t machines, std::size_t jobs, Distribution&& distribution)
    : m_machines(machines)
    , m_jobs(jobs)
    , m_distribution(std::forward<Distribution>(distribution))
    {
    }

    ParallelInstance operator()(Random& random) {
      std::vector<Time> times;

      for (std::size_t i = 0; i < m_jobs; ++i) {
        times.push_back(m_distribution(random));
      }

      return { m_machines, std::move(times) };
    }

  private:
    std::size_t m_machines;
    std::size_t m_jobs;
    Distribution m_distribution;
  };


  struct SCHED_API UniformParallelGenerator : GenericParallelGenerator<std::uniform_int_distribution<Time>> {
    UniformParallelGenerator(std::size_t machines, std::size_t jobs, Time min, Time max)
    : GenericParallelGenerator(machines, jobs, std::uniform_int_distribution<Time>(min, max))
    {
    }
  };

}

#endif // SCHED_PARA_GENERIC_PARALLEL_GENERATOR_H
