/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./random.hpp"
#include "./vector.hpp"

namespace tf {

/// @brief Generates a random vector of dimension `N` with values in the range [`from`, `to`].
///
/// Each component of the resulting vector is independently sampled using `tf::random(from, to)`.
/// Note: This overload requires that `N` is a known compile-time constant.
///
/// @tparam N Dimension of the vector.
/// @tparam T Numeric type of the vector components.
/// @param from The lower bound (inclusive) for each component.
/// @param to The upper bound (exclusive) for each component.
/// @return A `tf::vector<T, N>` filled with random values in [`from`, `to`].
///
/// @see tf::random
template <int N, typename T>
auto random_vector(T from, T to) -> tf::vector<T, N> {
  return tf::vector<T, N>{
      {tf::random(from, to), tf::random(from, to), tf::random(from, to)}};
}

/// @brief Generates a random vector of dimension `N` with values in the range [0, 1).
///
/// This is a convenience overload that calls `tf::random_vector<N>(T(0), T(1))`.
///
/// @tparam T Numeric type of the vector components.
/// @tparam N Dimension of the vector.
/// @return A `tf::vector<T, N>` filled with random values in [0, 1).
///
/// @see tf::random_vector
template <typename T, std::size_t N>
auto random_vector() -> tf::vector<T, N> {
  return tf::random_vector<N>(T(0), T(1));
}
} // namespace tf
