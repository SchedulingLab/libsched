// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_PARA_PARALLEL_INSTANCE_H
#define SCHED_PARA_PARALLEL_INSTANCE_H

#include <cassert>

#include <vector>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/Time.h>
#include <sched/para/schedule/ParallelSchedule.h>

namespace sched::para {

  struct SCHED_API ParallelInstance {
    using Schedule = ParallelSchedule;

    ParallelInstance(std::size_t machines, const std::vector<Time>& jobs)
    : m_machines(machines)
    , m_jobs(jobs)
    {
    }

    ParallelInstance(std::size_t machines, std::vector<Time>&& jobs)
    : m_machines(machines)
    , m_jobs(std::move(jobs))
    {
    }

    std::size_t machine_count() const noexcept
    {
      return m_machines;
    }

    std::size_t job_count() const noexcept
    {
      return m_jobs.size();
    }

    Time processing_time(JobId job, [[maybe_unused]] MachineId machine) const
    {
      assert(machine == AnyMachine);
      return m_jobs[to_index(job)];
    }

  private:
    std::size_t m_machines;
    std::vector<Time> m_jobs;
  };

}

#endif // SCHED_PARA_PARALLEL_INSTANCE_H
