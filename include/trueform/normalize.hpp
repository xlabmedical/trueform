/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./vector.hpp"
#include "./vector_view.hpp"

namespace tf {

/// @brief Normalizes a vector view in place.
///
/// Divides the vector by its Euclidean length, with protection against division
/// by zero. If the length is zero, the vector remains unchanged.
///
/// @tparam T The scalar type.
/// @tparam Dims The number of dimensions.
/// @param v A reference to the vector view to normalize.
/// @return Reference to the normalized vector view.
template <typename T, std::size_t Dims>
auto normalize(tf::vector_view<T, Dims> v) -> tf::vector_view<T, Dims> {
  auto d = v.length();
  v /= d + (d == 0);
  return v;
}

/// @brief Normalizes a vector in place by delegating to @ref
/// normalize(tf::vector_view<T, Dims>&).
///
/// Wraps the input vector in a @ref tf::vector_view and normalizes it in place.
///
/// @tparam T The scalar type.
/// @tparam Dims The number of dimensions.
/// @param v A reference to the vector to normalize.
/// @return Reference to the normalized vector.
template <typename T, std::size_t Dims>
auto normalize(tf::vector<T, Dims> &v) -> tf::vector<T, Dims> & {
  normalize(tf::make_vector_view<Dims>(v.begin()));
  return v;
}

} // namespace tf
