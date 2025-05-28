/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include <random>
#include <thread>

namespace tf {
namespace implementation {
template <typename Distribution>
auto distribution_sample(Distribution &distribution) {
  static thread_local std::mt19937 generator = std::mt19937(
      clock() + std::hash<std::thread::id>()(std::this_thread::get_id()));
  return distribution(generator);
}
} // namespace implementation

template <typename T> auto random(T from, T to) -> T {
  if constexpr (std::is_floating_point_v<T>) {
    std::uniform_real_distribution<T> distribution(from, to);
    return implementation::distribution_sample(distribution);
  } else {
    std::uniform_int_distribution<T> distribution(from, to);
    return implementation::distribution_sample(distribution);
  }
}

template <typename T> auto random() -> T { return tf::random(T(0), T(1)); }

} // namespace tf
