// clang-format off: main header
#include <sched/common/Random.h>
// clang-format on

#include <cinttypes>

namespace sched {

  namespace {

    constexpr uint64_t rotl(const uint64_t x, int k)
    {
      return (x << k) | (x >> (64 - k));
    }

    // https://prng.di.unimi.it/splitmix64.c
    struct SplitMix64 {
      constexpr SplitMix64(uint64_t seed)
      : m_state(seed)
      {
      }

      uint64_t next()
      {
        uint64_t z = (m_state += UINT64_C(0x9e3779b97f4a7c15));
        z = (z ^ (z >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
        z = (z ^ (z >> 27)) * UINT64_C(0x94d049bb133111eb);
        return z ^ (z >> 31);
      }

      uint64_t m_state;
    };

  }

  Random::Random(result_type seed)
  {
    SplitMix64 sm(seed);

    for (auto& state : m_state) {
      state = sm.next();
    }
  }

  auto Random::operator()() -> result_type
  {
    return next();
  }

  // https://prng.di.unimi.it/xoshiro256plusplus.c
  uint64_t Random::next()
  {
    const uint64_t result = rotl(m_state[0] + m_state[3], 23) + m_state[0];
    const uint64_t t = m_state[1] << 17;

    m_state[2] ^= m_state[0];
    m_state[3] ^= m_state[1];
    m_state[1] ^= m_state[2];
    m_state[0] ^= m_state[3];

    m_state[2] ^= t;

    m_state[3] = rotl(m_state[3], 45);

    return result;
  }

  void Random::jump()
  {
    static constexpr uint64_t Jump[] = {
      UINT64_C(0x180ec6d33cfd0aba),
      UINT64_C(0xd5a61266f0c9392c),
      UINT64_C(0xa9582618e03fc9aa),
      UINT64_C(0x39abdc4529b1661c)
    };

    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;

    for (auto jump : Jump) {
      for (int b = 0; b < 64; b++) {
        if (jump & UINT64_C(1) << b) {
          s0 ^= m_state[0];
          s1 ^= m_state[1];
          s2 ^= m_state[2];
          s3 ^= m_state[3];
        }

        next();
      }
    }

    m_state[0] = s0;
    m_state[1] = s1;
    m_state[2] = s2;
    m_state[3] = s3;
  }

  void Random::long_jump()
  {
    static constexpr uint64_t Jump[] = {
      UINT64_C(0x76e15d3efefdcbbf),
      UINT64_C(0xc5004e441c522fb3),
      UINT64_C(0x77710069854ee241),
      UINT64_C(0x39109bb02acbe635)
    };

    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;

    for (auto jump : Jump) {
      for (int b = 0; b < 64; b++) {
        if (jump & UINT64_C(1) << b) {
          s0 ^= m_state[0];
          s1 ^= m_state[1];
          s2 ^= m_state[2];
          s3 ^= m_state[3];
        }

        next();
      }
    }

    m_state[0] = s0;
    m_state[1] = s1;
    m_state[2] = s2;
    m_state[3] = s3;
  }

}
