/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./vector.hpp"
#include "./vector_view.hpp"

namespace tf {
/// @brief Compute the dot product of two N-dimensional vectors.
///
/// Returns the standard inner product (a · b) between two vectors
/// of the same dimension.
///
/// @tparam T The scalar type of the vector components (e.g., float or double).
/// @tparam N The dimensionality of the vectors.
/// @param a The first input vector.
/// @param b The second input vector.
/// @return The dot product of vectors `a` and `b`.
template <typename T, std::size_t N>
auto dot(const vector_view<T, N> &a, const vector_view<T, N> &b) -> T {
  using val_t = std::decay_t<T>;
  val_t sum = val_t{};
  for (std::size_t i = 0; i < N; ++i) {
    sum += a[i] * b[i];
  }
  return sum;
}
/// @brief Compute the dot product of two N-dimensional vectors.
///
/// Returns the standard inner product (a · b) between two vectors
/// of the same dimension.
///
/// @tparam T The scalar type of the vector components (e.g., float or double).
/// @tparam N The dimensionality of the vectors.
/// @param a The first input vector.
/// @param b The second input vector.
/// @return The dot product of vectors `a` and `b`.
template <typename T, std::size_t N>
auto dot(const vector<T, N> &a, const vector<T, N> &b) -> T {
  return dot(tf::make_vector_view<N>(a.begin()),
             tf::make_vector_view<N>(b.begin()));
}
/// @brief Compute the dot product of two N-dimensional vectors.
///
/// Returns the standard inner product (a · b) between two vectors
/// of the same dimension.
///
/// @tparam T The scalar type of the vector components (e.g., float or double).
/// @tparam N The dimensionality of the vectors.
/// @param a The first input vector.
/// @param b The second input vector.
/// @return The dot product of vectors `a` and `b`.
template <typename T, std::size_t N>
auto dot(const vector_view<T, N> &a, const vector<T, N> &b) -> T {
  return dot(a, tf::make_vector_view<N>(b.begin()));
}
/// @brief Compute the dot product of two N-dimensional vectors.
///
/// Returns the standard inner product (a · b) between two vectors
/// of the same dimension.
///
/// @tparam T The scalar type of the vector components (e.g., float or double).
/// @tparam N The dimensionality of the vectors.
/// @param a The first input vector.
/// @param b The second input vector.
/// @return The dot product of vectors `a` and `b`.
template <typename T, std::size_t N>
auto dot(const vector<T, N> &a, const vector_view<T, N> &b) -> T {
  return dot(tf::make_vector_view<N>(a.begin()), b);
}
} // namespace tf
