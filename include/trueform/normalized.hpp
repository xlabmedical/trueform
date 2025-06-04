/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./normalize.hpp"

namespace tf {
/// @brief Return a normalized copy of a vector view.
///
/// Creates a copy of the input @ref tf::vector_view, normalizes it using @ref normalize(),
/// and returns the result. The original input remains unchanged.
///
/// @tparam T The scalar type (e.g., float or double).
/// @tparam Dims The dimensionality of the vector.
/// @param v The input vector view to normalize.
/// @return A normalized vector of type @ref tf::vector<T, Dims>.
template <typename T, std::size_t Dims>
auto normalized(const tf::vector_view<T, Dims> &v) -> tf::vector<T, Dims> {
  auto out = v;
  return normalize(out);
}

/// @brief Return a normalized copy of a vector.
///
/// Creates a copy of the input @ref tf::vector, normalizes it using @ref normalize(),
/// and returns the result. The original input remains unchanged.
///
/// @tparam T The scalar type (e.g., float or double).
/// @tparam Dims The dimensionality of the vector.
/// @param v The input vector to normalize.
/// @return A normalized vector of type @ref tf::vector<T, Dims>.
template <typename T, std::size_t Dims>
auto normalized(const tf::vector<T, Dims> &v) -> tf::vector<T, Dims> {
  auto out = v;
  return normalize(out);
}

} // namespace tf
