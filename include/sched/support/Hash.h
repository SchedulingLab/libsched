// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_HASH_H
#define SCHED_HASH_H

#include <cstddef>
#include <cstdint>

#include <span>
#include <utility>

namespace sched {

  constexpr uint64_t hash_bytes(std::span<const std::byte> bytes)
  {
    uint64_t value = UINT64_C(0xCBF29CE484222325);

    for (std::byte byte : bytes) {
      value ^= std::to_underlying(byte);
      value *= UINT64_C(0x100000001B3);
    }

    return value;
  }

  // based on https://github.com/boostorg/container_hash/blob/7288df8beea1c3c8222cd48af1c07c589f7d3f8a/include/boost/container_hash/detail/hash_mix.hpp#L69
  // License: Boost Software License, Version 1.0.
  constexpr uint64_t hash_mix(uint64_t x)
  {
    constexpr uint64_t const M = UINT64_C(0xE9846AF9B1A615D);
    x ^= x >> 32;
    x *= M;
    x ^= x >> 32;
    x *= M;
    x ^= x >> 28;
    return x;
  }

  // based on https://github.com/boostorg/container_hash/blob/7288df8beea1c3c8222cd48af1c07c589f7d3f8a/include/boost/container_hash/hash.hpp#L469
  // License: Boost Software License, Version 1.0.
  constexpr uint64_t hash_combine(uint64_t state, uint64_t value)
  {
    return hash_mix(state + UINT64_C(0x9E3779B9) + value);
  }

}

#endif // SCHED_HASH_H
