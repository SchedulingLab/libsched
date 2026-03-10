// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <cstdint>
#include <cmath>

#include <algorithm>
#include <ranges>

#include <sched/shop/instance/JobShopInstanceGenerator.h>

namespace sched::shop {

  /*
   * Taillard: adapted from https://people.brunel.ac.uk/~mastjjb/jeb/orlib/files/jobshop2.txt
   */

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

  /*
   * DMU: adapted from https://web.archive.org/web/20021208153456/http://palette.ecn.purdue.edu/~uzsoy2/benchmark/code/code.html
   */

  namespace {

    float dmu_uniform(int32_t& seed)
    {
      static constexpr int64_t M = 2147483647;
      static constexpr int64_t A = 16807;

      const int64_t extended_seed = (seed * A) % M;
      seed = static_cast<int32_t>(extended_seed);
      return static_cast<float>(extended_seed) / static_cast<float>(M + 1);
    }

    constexpr int32_t DmuMinProcessingTime = 1;
    constexpr int32_t DmuProcessingTimeRange = 200;

  }

  JobShopInstance compute_dmu_legacy_instance_1(const DemirkolMehtaUzsoyInstance& instance)
  {
    JobShopData instance_data;
    instance_data.machines = instance.machines;

    int32_t seed = instance.seed;

    struct MachineValue {
      MachineId machine;
      float value;
    };

    for (std::size_t job = 0; job < instance.jobs; ++job) {
      JobData job_data;

      std::vector<MachineValue> sequence;
      sequence.reserve(instance.machines);

      for (std::size_t i = 0; i < instance.machines; ++i) {
        sequence.emplace_back(to_machine(i), dmu_uniform(seed));
      }

      std::ranges::sort(sequence, {}, &MachineValue::value);

      for (std::size_t i = 0; i < instance.machines; ++i) {
        const OperationData operation_data = {
          .machine = sequence[i].machine,
          .processing = static_cast<Time>(DmuMinProcessingTime + (dmu_uniform(seed) * DmuProcessingTimeRange))
        };

        job_data.operations.push_back(operation_data);
      }

      instance_data.jobs.push_back(std::move(job_data));
    }

    return instance_data;
  }

  JobShopInstance compute_dmu_legacy_instance_2(const DemirkolMehtaUzsoyInstance& instance)
  {
    JobShopData instance_data;
    instance_data.machines = instance.machines;

    int32_t seed = instance.seed;
    const std::size_t half = instance.machines / 2;

    struct MachineValue {
      MachineId machine;
      float value;
    };

    for (std::size_t job = 0; job < instance.jobs; ++job) {
      JobData job_data;

      std::vector<MachineValue> sequence;
      sequence.reserve(instance.machines);

      for (std::size_t i = 0; i < instance.machines; ++i) {
        sequence.emplace_back(to_machine(i), dmu_uniform(seed));
      }

      std::ranges::sort(sequence | std::views::take(half), {}, &MachineValue::value);
      std::ranges::sort(sequence | std::views::drop(half), {}, &MachineValue::value);

      for (std::size_t i = 0; i < instance.machines; ++i) {
        const OperationData operation_data = {
          .machine = sequence[i].machine,
          .processing = static_cast<Time>(DmuMinProcessingTime + (dmu_uniform(seed) * DmuProcessingTimeRange))
        };

        job_data.operations.push_back(operation_data);
      }

      instance_data.jobs.push_back(std::move(job_data));
    }

    return instance_data;
  }

  namespace {

    constexpr DemirkolMehtaUzsoyInstance DmuInstances1[] = {
      { .jobs = 20, .machines = 15, .seed =  155125852 },
      { .jobs = 20, .machines = 15, .seed = 2029877925 },
      { .jobs = 20, .machines = 15, .seed =  196931796 },
      { .jobs = 20, .machines = 15, .seed = 2109450727 },
      { .jobs = 20, .machines = 15, .seed = 1017664806 },
      { .jobs = 20, .machines = 20, .seed =  704263516 },
      { .jobs = 20, .machines = 20, .seed =  658766149 },
      { .jobs = 20, .machines = 20, .seed = 1918985685 },
      { .jobs = 20, .machines = 20, .seed =  263171901 },
      { .jobs = 20, .machines = 20, .seed = 1630638936 },
      { .jobs = 30, .machines = 15, .seed =  311313093 },
      { .jobs = 30, .machines = 15, .seed =  316280323 },
      { .jobs = 30, .machines = 15, .seed = 1237221727 },
      { .jobs = 30, .machines = 15, .seed =  982174120 },
      { .jobs = 30, .machines = 15, .seed =  899442265 },
      { .jobs = 30, .machines = 20, .seed = 1970680013 },
      { .jobs = 30, .machines = 20, .seed =  491806903 },
      { .jobs = 30, .machines = 20, .seed =    7335742 },
      { .jobs = 30, .machines = 20, .seed =  554421187 },
      { .jobs = 30, .machines = 20, .seed = 1227925003 },
      { .jobs = 40, .machines = 15, .seed =  180026511 },
      { .jobs = 40, .machines = 15, .seed =   86633815 },
      { .jobs = 40, .machines = 15, .seed =  800808272 },
      { .jobs = 40, .machines = 15, .seed = 2104225727 },
      { .jobs = 40, .machines = 15, .seed = 1193746195 },
      { .jobs = 40, .machines = 20, .seed = 1510794207 },
      { .jobs = 40, .machines = 20, .seed = 1656193480 },
      { .jobs = 40, .machines = 20, .seed = 1020622462 },
      { .jobs = 40, .machines = 20, .seed =  425341562 },
      { .jobs = 40, .machines = 20, .seed = 1111362916 },
      { .jobs = 50, .machines = 15, .seed = 1140902718 },
      { .jobs = 50, .machines = 15, .seed =  319220731 },
      { .jobs = 50, .machines = 15, .seed = 1200567745 },
      { .jobs = 50, .machines = 15, .seed =  408890368 },
      { .jobs = 50, .machines = 15, .seed =  668339743 },
      { .jobs = 50, .machines = 20, .seed = 1353154750 },
      { .jobs = 50, .machines = 20, .seed = 1756084517 },
      { .jobs = 50, .machines = 20, .seed =  444307538 },
      { .jobs = 50, .machines = 20, .seed =  618487528 },
      { .jobs = 50, .machines = 20, .seed = 1509456066 },
    };

    constexpr DemirkolMehtaUzsoyInstance DmuInstances2[] = {
      { .jobs = 20, .machines = 15, .seed =  963246540 },
      { .jobs = 20, .machines = 15, .seed =  571501185 },
      { .jobs = 20, .machines = 15, .seed = 1064792308 },
      { .jobs = 20, .machines = 15, .seed =  500883009 },
      { .jobs = 20, .machines = 15, .seed =  486487743 },
      { .jobs = 20, .machines = 20, .seed =  366904335 },
      { .jobs = 20, .machines = 20, .seed =  193063992 },
      { .jobs = 20, .machines = 20, .seed = 1708102606 },
      { .jobs = 20, .machines = 20, .seed = 1111794661 },
      { .jobs = 20, .machines = 20, .seed = 1759497606 },
      { .jobs = 30, .machines = 15, .seed = 1085428258 },
      { .jobs = 30, .machines = 15, .seed =  808544005 },
      { .jobs = 30, .machines = 15, .seed =  810658329 },
      { .jobs = 30, .machines = 15, .seed =  142450034 },
      { .jobs = 30, .machines = 15, .seed =  694627358 },
      { .jobs = 30, .machines = 20, .seed = 1853446555 },
      { .jobs = 30, .machines = 20, .seed = 1696470024 },
      { .jobs = 30, .machines = 20, .seed =  852368975 },
      { .jobs = 30, .machines = 20, .seed = 1122276408 },
      { .jobs = 30, .machines = 20, .seed =  169103059 },
      { .jobs = 40, .machines = 15, .seed = 1184095505 },
      { .jobs = 40, .machines = 15, .seed = 1032938603 },
      { .jobs = 40, .machines = 15, .seed = 2047416508 },
      { .jobs = 40, .machines = 15, .seed =  735586133 },
      { .jobs = 40, .machines = 15, .seed = 1202629385 },
      { .jobs = 40, .machines = 20, .seed =  295809381 },
      { .jobs = 40, .machines = 20, .seed =  997471796 },
      { .jobs = 40, .machines = 20, .seed = 1524255499 },
      { .jobs = 40, .machines = 20, .seed =  229789972 },
      { .jobs = 40, .machines = 20, .seed = 1780414345 },
      { .jobs = 50, .machines = 15, .seed = 1936600566 },
      { .jobs = 50, .machines = 15, .seed =  567182173 },
      { .jobs = 50, .machines = 15, .seed =  363888850 },
      { .jobs = 50, .machines = 15, .seed = 1103051921 },
      { .jobs = 50, .machines = 15, .seed = 2084553596 },
      { .jobs = 50, .machines = 20, .seed = 1215157252 },
      { .jobs = 50, .machines = 20, .seed =  718365005 },
      { .jobs = 50, .machines = 20, .seed =  499659734 },
      { .jobs = 50, .machines = 20, .seed = 1315007823 },
      { .jobs = 50, .machines = 20, .seed =  556673305 },
    };

  }

  SCHED_API std::span<const DemirkolMehtaUzsoyInstance, DmuInstanceCount> load_dmu_benchmark_1()
  {
    return DmuInstances1;
  }

  SCHED_API std::span<const DemirkolMehtaUzsoyInstance, DmuInstanceCount> load_dmu_benchmark_2()
  {
    return DmuInstances2;
  }

}
