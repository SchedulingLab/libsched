// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <cstdint>
#include <cmath>

#include <sched/shop/instance/JobShopInstanceGenerator.h>
#include "sched/shop/instance/JobShopData.h"

namespace sched::shop {

  namespace {

    int32_t taillard_uniform(int32_t& seed, int32_t low, int32_t high)
    {
      static constexpr int32_t M = 2147483647;
      static constexpr int32_t A = 16807;
      static constexpr int32_t B = 127773;
      static constexpr int32_t C = 2836;

      const int32_t k = seed / B;
      seed = (A * (seed % B)) - (k * C);

      if (seed < 0) {
        seed += M;
      }

      const double value_0_1 = static_cast<double>(seed) / static_cast<double>(M);
      return low + static_cast<int32_t>(value_0_1 * (high - low + 1));
    }

    constexpr int32_t TaillardMinProcessingTime = 1;
    constexpr int32_t TaillardMaxProcessingTime = 99;

  }

  JobShopInstance compute_taillard_legacy_instance(const TaillardInstance& instance)
  {
    JobShopData instance_data;
    instance_data.machines = instance.machines;

    int32_t time_seed = instance.time_seed;
    int32_t machine_seed = instance.machine_seed;

    for (std::size_t job = 0; job < instance.jobs; ++job) {
      JobData job_data;

      for (std::size_t machine = 0; machine < instance.machines; ++machine) {
        const OperationData operation_data = {
          .machine = to_machine(machine),
          .processing = taillard_uniform(time_seed, TaillardMinProcessingTime, TaillardMaxProcessingTime)
        };

        job_data.operations.push_back(operation_data);
      }

      instance_data.jobs.push_back(std::move(job_data));
    }

    for (std::size_t job = 0; job < instance.jobs; ++job) {
      for (std::size_t machine = 0; machine < instance.machines; ++machine) {
        const std::size_t other = taillard_uniform(machine_seed, static_cast<int32_t>(machine), static_cast<int32_t>(instance.machines - 1));
        std::swap(instance_data.jobs[job].operations[machine].machine, instance_data.jobs[job].operations[other].machine);
      }
    }

    return instance_data;
  }

  namespace {

    constexpr TaillardInstance TaillardInstances[] = {
      { .jobs =  15, .machines = 15, .time_seed =  840612802, .machine_seed =  398197754  },
      { .jobs =  15, .machines = 15, .time_seed = 1314640371, .machine_seed =  386720536  },
      { .jobs =  15, .machines = 15, .time_seed = 1227221349, .machine_seed =  316176388  },
      { .jobs =  15, .machines = 15, .time_seed =  342269428, .machine_seed = 1806358582  },
      { .jobs =  15, .machines = 15, .time_seed = 1603221416, .machine_seed = 1501949241  },
      { .jobs =  15, .machines = 15, .time_seed = 1357584978, .machine_seed = 1734077082  },
      { .jobs =  15, .machines = 15, .time_seed =   44531661, .machine_seed = 1374316395  },
      { .jobs =  15, .machines = 15, .time_seed =  302545136, .machine_seed = 2092186050  },
      { .jobs =  15, .machines = 15, .time_seed = 1153780144, .machine_seed = 1393392374  },
      { .jobs =  15, .machines = 15, .time_seed =   73896786, .machine_seed = 1544979948  },
      { .jobs =  20, .machines = 15, .time_seed =  533484900, .machine_seed =  317419073  },
      { .jobs =  20, .machines = 15, .time_seed = 1894307698, .machine_seed = 1474268163  },
      { .jobs =  20, .machines = 15, .time_seed =  874340513, .machine_seed =  509669280  },
      { .jobs =  20, .machines = 15, .time_seed = 1124986343, .machine_seed = 1209573668  },
      { .jobs =  20, .machines = 15, .time_seed = 1463788335, .machine_seed =  529048107  },
      { .jobs =  20, .machines = 15, .time_seed = 1056908795, .machine_seed =   25321885  },
      { .jobs =  20, .machines = 15, .time_seed =  195672285, .machine_seed = 1717580117  },
      { .jobs =  20, .machines = 15, .time_seed =  961965583, .machine_seed = 1353003786  },
      { .jobs =  20, .machines = 15, .time_seed = 1610169733, .machine_seed = 1734469503  },
      { .jobs =  20, .machines = 15, .time_seed =  532794656, .machine_seed =  998486810  },
      { .jobs =  20, .machines = 20, .time_seed = 1035939303, .machine_seed =  773961798  },
      { .jobs =  20, .machines = 20, .time_seed =    5997802, .machine_seed = 1872541150  },
      { .jobs =  20, .machines = 20, .time_seed = 1357503601, .machine_seed =  722225039  },
      { .jobs =  20, .machines = 20, .time_seed =  806159563, .machine_seed = 1166962073  },
      { .jobs =  20, .machines = 20, .time_seed = 1902815253, .machine_seed = 1879990068  },
      { .jobs =  20, .machines = 20, .time_seed = 1503184031, .machine_seed = 1850351876  },
      { .jobs =  20, .machines = 20, .time_seed = 1032645967, .machine_seed =   99711329  },
      { .jobs =  20, .machines = 20, .time_seed =  229894219, .machine_seed = 1158117804  },
      { .jobs =  20, .machines = 20, .time_seed =  823349822, .machine_seed =  108033225  },
      { .jobs =  20, .machines = 20, .time_seed = 1297900341, .machine_seed =  489486403  },
      { .jobs =  30, .machines = 15, .time_seed =   98640593, .machine_seed = 1981283465  },
      { .jobs =  30, .machines = 15, .time_seed = 1839268120, .machine_seed =  248890888  },
      { .jobs =  30, .machines = 15, .time_seed =  573875290, .machine_seed = 2081512253  },
      { .jobs =  30, .machines = 15, .time_seed = 1670898570, .machine_seed =  788294565  },
      { .jobs =  30, .machines = 15, .time_seed = 1118914567, .machine_seed = 1074349202  },
      { .jobs =  30, .machines = 15, .time_seed =  178750207, .machine_seed =  294279708  },
      { .jobs =  30, .machines = 15, .time_seed = 1549372605, .machine_seed =  596993084  },
      { .jobs =  30, .machines = 15, .time_seed =  798174738, .machine_seed =  151685779  },
      { .jobs =  30, .machines = 15, .time_seed =  553410952, .machine_seed = 1329272528  },
      { .jobs =  30, .machines = 15, .time_seed = 1661531649, .machine_seed = 1173386294  },
      { .jobs =  30, .machines = 20, .time_seed = 1841414609, .machine_seed = 1357882888  },
      { .jobs =  30, .machines = 20, .time_seed = 2116959593, .machine_seed = 1546338557  },
      { .jobs =  30, .machines = 20, .time_seed =  796392706, .machine_seed = 1230864158  },
      { .jobs =  30, .machines = 20, .time_seed =  532496463, .machine_seed =  254174057  },
      { .jobs =  30, .machines = 20, .time_seed = 2020525633, .machine_seed =  978943053  },
      { .jobs =  30, .machines = 20, .time_seed =  524444252, .machine_seed =  185526083  },
      { .jobs =  30, .machines = 20, .time_seed = 1569394691, .machine_seed =  487269855  },
      { .jobs =  30, .machines = 20, .time_seed = 1460267840, .machine_seed = 1631446539  },
      { .jobs =  30, .machines = 20, .time_seed =  198324822, .machine_seed = 1937476577  },
      { .jobs =  30, .machines = 20, .time_seed =   38071822, .machine_seed = 1541985579  },
      { .jobs =  50, .machines = 15, .time_seed =      17271, .machine_seed =     718939  },
      { .jobs =  50, .machines = 15, .time_seed =  660481279, .machine_seed =  449650254  },
      { .jobs =  50, .machines = 15, .time_seed =  352229765, .machine_seed =  949737911  },
      { .jobs =  50, .machines = 15, .time_seed = 1197518780, .machine_seed =  166840558  },
      { .jobs =  50, .machines = 15, .time_seed = 1376020303, .machine_seed =  483922052  },
      { .jobs =  50, .machines = 15, .time_seed = 2106639239, .machine_seed =  955932362  },
      { .jobs =  50, .machines = 15, .time_seed = 1765352082, .machine_seed = 1209982549  },
      { .jobs =  50, .machines = 15, .time_seed = 1105092880, .machine_seed = 1349003108  },
      { .jobs =  50, .machines = 15, .time_seed =  907248070, .machine_seed =  919544535  },
      { .jobs =  50, .machines = 15, .time_seed = 2011630757, .machine_seed = 1845447001  },
      { .jobs =  50, .machines = 20, .time_seed =    8493988, .machine_seed =    2738939  },
      { .jobs =  50, .machines = 20, .time_seed = 1991925010, .machine_seed =  709517751  },
      { .jobs =  50, .machines = 20, .time_seed =  342093237, .machine_seed =  786960785  },
      { .jobs =  50, .machines = 20, .time_seed = 1634043183, .machine_seed =  973178279  },
      { .jobs =  50, .machines = 20, .time_seed =  341706507, .machine_seed =  286513148  },
      { .jobs =  50, .machines = 20, .time_seed =  320167954, .machine_seed = 1411193018  },
      { .jobs =  50, .machines = 20, .time_seed = 1089696753, .machine_seed =  298068750  },
      { .jobs =  50, .machines = 20, .time_seed =  433032965, .machine_seed = 1589656152  },
      { .jobs =  50, .machines = 20, .time_seed =  615974477, .machine_seed =  331205412  },
      { .jobs =  50, .machines = 20, .time_seed =  236150141, .machine_seed =  592292984  },
      { .jobs = 100, .machines = 20, .time_seed =  302034063, .machine_seed = 1203569070 },
      { .jobs = 100, .machines = 20, .time_seed = 1437643198, .machine_seed = 1692025209 },
      { .jobs = 100, .machines = 20, .time_seed = 1792475497, .machine_seed = 1039908559 },
      { .jobs = 100, .machines = 20, .time_seed = 1647273132, .machine_seed = 1012841433 },
      { .jobs = 100, .machines = 20, .time_seed =  696480901, .machine_seed = 1689682358 },
      { .jobs = 100, .machines = 20, .time_seed = 1785569423, .machine_seed = 1092647459 },
      { .jobs = 100, .machines = 20, .time_seed =  117806902, .machine_seed =  739059626 },
      { .jobs = 100, .machines = 20, .time_seed = 1639154709, .machine_seed = 1319962509 },
      { .jobs = 100, .machines = 20, .time_seed = 2007423389, .machine_seed =  749368241 },
      { .jobs = 100, .machines = 20, .time_seed =  682761130, .machine_seed =  262763021 },
    };

  }

  std::span<const TaillardInstance, TaillardInstanceCount> load_taillard_benchmark()
  {
    return TaillardInstances;
  }

}
