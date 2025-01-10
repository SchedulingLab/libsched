#ifndef SCHED_PARA_LDM_ALGORITHM_H
#define SCHED_PARA_LDM_ALGORITHM_H

#include <cassert>

#include <algorithm>
#include <vector>

#include <sched/Api.h>
#include <sched/Instance.h>
#include <sched/para/helper/ParallelJob.h>
#include <sched/para/schedule/ParallelSchedule.h>

namespace sched::para {

  namespace details {

    struct LdmSet {
      std::vector<ParallelJob> members;
      Time total = 0;

      LdmSet() = default;

      LdmSet(ParallelJob job)
      : members({ job })
      , total(job.processing_time)
      {
      }
    };

    inline LdmSet ldm_set_merge(const LdmSet& lhs, const LdmSet& rhs)
    {
      LdmSet result;
      result.members.insert(result.members.end(), lhs.members.begin(), lhs.members.end());
      result.members.insert(result.members.end(), rhs.members.begin(), rhs.members.end());
      result.total = lhs.total + rhs.total;
      return result;
    }

    struct LdmTuple {
      std::vector<LdmSet> sets;

      LdmTuple() = default;

      LdmTuple(std::size_t machine_count, ParallelJob job)
      : sets(machine_count - 1)
      {
        sets.emplace_back(job);
      }
    };

    inline LdmTuple ldm_tuple_merge(const LdmTuple& lhs, const LdmTuple& rhs)
    {
      const std::size_t count = lhs.sets.size();
      assert(rhs.sets.size() == count);

      LdmTuple result;

      for (std::size_t i = 0; i < count; ++i) {
        result.sets.push_back(ldm_set_merge(lhs.sets[i], rhs.sets[count - i - 1]));
      }

      assert(result.sets.size() == count);

      std::ranges::sort(result.sets, [](const LdmSet& rhs, const LdmSet& lhs) {
        return rhs.total < lhs.total;
      });

      return result;
    }

    struct LdmPartition {
      std::vector<LdmTuple> tuples;
      std::size_t machine_count;

      LdmPartition(std::size_t count)
      : machine_count(count)
      {
      }

      void append(ParallelJob job)
      {
        tuples.emplace_back(machine_count, job);
      }

      void run()
      {
        auto comparator = [](const LdmTuple& lhs, const LdmTuple& rhs) {
          return (lhs.sets.back().total - lhs.sets.front().total) < (rhs.sets.back().total - rhs.sets.front().total);
        };

        std::sort(tuples.begin(), tuples.end(), comparator);

        while (tuples.size() > 1) {
          const LdmTuple last1 = std::move(tuples.back());
          tuples.pop_back();
          const LdmTuple last2 = std::move(tuples.back());
          tuples.pop_back();

          LdmTuple merged = ldm_tuple_merge(last1, last2);

          auto it = std::lower_bound(tuples.begin(), tuples.end(), merged, comparator);
          tuples.insert(it, std::move(merged));
        }
      }
    };

  }

  struct SCHED_API LargestDifferencingMethodAlgorithm {

    template<typename Instance>
    ParallelSchedule operator()(const Instance& instance)
    {
      details::LdmPartition partition(instance.machine_count());

      for (auto job : sched::jobs(instance)) {
        partition.append({ job, instance.processing_time(job, AnyMachine) });
      }

      partition.run();

      assert(partition.tuples.size() == 1);

      ParallelSchedule schedule;

      std::size_t machine = 0;

      for (const details::LdmSet& set : partition.tuples.front().sets) {
        Time time = 0;

        for (const ParallelJob& job : set.members) {
          ParallelTask task = {};
          task.job = job.id;
          task.machine = MachineId{ machine };
          task.start = time;
          task.completion = task.start + job.processing_time;
          schedule.append(task);

          time += job.processing_time;
        }

        ++machine;
      }

      return schedule;
    }
  };

  using LdmAlgorithm = LargestDifferencingMethodAlgorithm;

}

#endif // SCHED_PARA_LDM_ALGORITHM_H
