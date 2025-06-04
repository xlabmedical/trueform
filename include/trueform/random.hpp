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

/// @brief Generate a random value in a specified range.
///
/// Produces a random value of type `T` uniformly distributed in the interval [`from`, `to`].
///
/// @tparam T A numeric type supporting uniform distribution (e.g., `float`, `double`, or integral).
/// @param from Lower bound of the range (inclusive).
/// @param to Upper bound of the range (inclusive or exclusive depending on `T`).
/// @return A randomly generated value of type `T`.
template <typename T> auto random(T from, T to) -> T {
  if constexpr (std::is_floating_point_v<T>) {
    std::uniform_real_distribution<T> distribution(from, to);
    return implementation::distribution_sample(distribution);
  } else {
    std::uniform_int_distribution<T> distribution(from, to);
    return implementation::distribution_sample(distribution);
  }
}

/// @brief Generate a random value in the default range [`0`, `1`].
///
/// Equivalent to `tf::random(T(0), T(1))`. Useful for floating-point types.
///
/// @tparam T A numeric type supporting uniform distribution.
/// @return A randomly generated value of type `T` in the default range.
template <typename T> auto random() -> T { return tf::random(T(0), T(1)); }

} // namespace tf
