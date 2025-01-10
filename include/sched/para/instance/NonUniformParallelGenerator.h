#ifndef SCHED_PARA_NON_UNIFORM_PARALLEL_GENERATOR_H
#define SCHED_PARA_NON_UNIFORM_PARALLEL_GENERATOR_H

#include <random>

#include <sched/Api.h>
#include <sched/Time.h>
#include <sched/support/Random.h>
#include <sched/para/instance/ParallelInstance.h>

namespace sched::para {

  struct SCHED_API NonUniformParallelGenerator {
    NonUniformParallelGenerator(std::size_t machines, std::size_t jobs, Time min, Time max)
    : m_machines(machines)
    , m_jobs(jobs)
    , m_min(min)
    , m_max(max)
    {
    }

    ParallelInstance operator()(Random& random) const
    {
      const Time range = m_max - m_min;
      const Time lower = range - (range / 10);  // should be m_min + range - range / 10
      const Time upper = range / 5;             // should be m_min + range / 5

      const std::size_t minority = m_jobs / 50;
      const std::size_t majority = m_jobs - minority;

      std::vector<Time> times;
      times.reserve(majority + minority);

      std::uniform_int_distribution<Time> big(lower, m_max);

      for (std::size_t i = 0; i < majority; ++i) {
        times.push_back(big(random));
      }

      std::uniform_int_distribution<Time> small(m_min, upper);

      for (std::size_t i = 0; i < minority; ++i) {
        times.push_back(small(random));
      }

      return { m_machines, std::move(times) };
    }

  private:
    std::size_t m_machines;
    std::size_t m_jobs;
    Time m_min;
    Time m_max;
  };

}

#endif // SCHED_PARA_NON_UNIFORM_PARALLEL_GENERATOR_H
