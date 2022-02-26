#ifndef SCHED_ENUMERATOR_H
#define SCHED_ENUMERATOR_H

#include <algorithm>
#include <limits>
#include <fstream>
#include <map>
#include <vector>

#include "JobShopSchedule.h"

namespace sched::shop {

  template<typename Instance, typename Input, typename Engine, typename Neighborhood, typename Criterion>
  struct Enumerator {

    std::map<JobShopSchedule, std::vector<Input>> operator()(const Instance& instance) {
      Input input = engine.generate_input(instance);
      std::size_t count = 0;
      std::size_t feasible = 0;
      std::vector<Input> infeasible;
      std::map<JobShopSchedule, std::vector<Input>> scheds;

      enumerate_start(input);

      do {
        ++count;
        auto sched = engine(instance, input);

        if (sched) {
          ++feasible;
          scheds[*sched].push_back(input);
        } else {
          infeasible.push_back(input);
        }
      } while (enumerate_next(input));

//       std::cout << count << ' ' << feasible << ' '  << scheds.size() << '\n';

      scheds.emplace(JobShopSchedule{}, std::move(infeasible));
      return scheds;
    }

    void to_dot(const Instance& instance, const std::map<JobShopSchedule, std::vector<Input>>& scheds, const std::string& filename) {
      struct Report {
        std::size_t id;
        Time time;
      };

      std::map<JobShopSchedule, Report> reports;
      std::size_t id = 0;
      Time opt = std::numeric_limits<Time>::max();

      for (auto& [ sched, inputs ] : scheds) {
        Report report;
        report.id = id++;
        report.time = criterion(instance, sched);

        if (!sched.works.empty() && report.time < opt) {
          opt = report.time;
        }

        reports.insert({ sched, report });
      }

      std::ofstream out(filename);

      out << "graph JobShop {\tnode [ style=filled ];\n";

      for (auto& [ sched, inputs ] : scheds) {

        out << '\t' << reports[sched].id << " [label=" << inputs.size();

        if (sched.works.empty()) {
          out << ", color=red";
        } else if (reports[sched].time == opt) {
          out << ", color=green";
        }

        out << "];\n";
      }

      std::vector<int> distribution;

      for (auto& [ sched0, inputs0 ] : scheds) {
        for (auto& [ sched1, inputs1 ] : scheds) {
          if (sched1 < sched0 || sched1 == sched0) {
            continue;
          }

          int neighbors = 0;

          for (auto input0 : inputs0) {
            for (auto input1 : inputs1) {
              if (neighborhood.are_neighbors(input0, input1)) {
                ++neighbors;
              }
            }
          }

          if (neighbors > 0) {
            distribution.push_back(neighbors);
          }
        }
      }

      std::sort(distribution.begin(), distribution.end());
      int limit = distribution[0.9 * distribution.size()];

      for (auto& [ sched0, inputs0 ] : scheds) {
        for (auto& [ sched1, inputs1 ] : scheds) {
          if (sched1 < sched0 || sched1 == sched0) {
            continue;
          }

          int neighbors = 0;

          for (auto input0 : inputs0) {
            for (auto input1 : inputs1) {
              if (neighborhood.are_neighbors(input0, input1)) {
                ++neighbors;
              }
            }
          }

          if (neighbors > 0) {
            out << reports[sched0].id << " -- " << reports[sched1].id;

            if (neighbors < limit) {
              out << " [color=lightgray]";
            } else {
              out << " [label=" << neighbors << "]";
            }

            out << ";\n";
          }
        }
      }

      out << "}\n";
    }

    Neighborhood neighborhood;
    Engine engine;
    Criterion criterion;
  };

}

#endif // SCHED_ENUMERATOR_H


