/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./point_like.hpp"
#include "./value_type.hpp"
#include "./vector_like.hpp"

namespace tf {
/// @ingroup geometry
/// @brief Compute the dot product of two N-dimensional vectors.
///
/// Returns the standard inner product (a · b) between two vectors
/// of the same dimension.
///
/// @tparam N The dimensionality of the vectors.
/// @tparam T0 The vector policy
/// @tparam T1 The vector policy
/// @param a The first input vector.
/// @param b The second input vector.
/// @return The dot product of vectors `a` and `b`.
template <std::size_t N, typename T0, typename T1>
auto dot(const vector_like<N, T0> &a, const vector_like<N, T1> &b)
    -> tf::common_value<T0, T1> {
  tf::common_value<T0, T1> sum{0};
  for (std::size_t i = 0; i < N; ++i) {
    sum += a[i] * b[i];
  }
  return sum;
}

template <std::size_t N, typename T0, typename T1>
auto dot(const point_like<N, T0> &a, const vector_like<N, T1> &b)
    -> tf::common_value<T0, T1> {
  return dot(a.as_vector_view(), b);
}

template <std::size_t N, typename T0, typename T1>
auto dot(const vector_like<N, T0> &a, const point_like<N, T1> &b)
    -> tf::common_value<T0, T1> {
  return dot(a, b.as_vector_view());
}
} // namespace tf
