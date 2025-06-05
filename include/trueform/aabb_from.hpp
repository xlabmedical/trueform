/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./aabb.hpp"
#include "./vector.hpp"
#include "./vector_view.hpp"

namespace tf {

/// @ingroup geometry
/// @brief Construct an AABB from a single point.
///
/// The resulting AABB has identical `min` and `max` bounds equal to the input
/// point.
///
/// @tparam T The scalar type (e.g., float or double).
/// @tparam N The number of dimensions (e.g., 2 or 3).
/// @param pt The input point.
/// @return An axis-aligned bounding box with zero extent at the point.
template <typename T, std::size_t N>
auto aabb_from(const vector<T, N> &pt) -> aabb<T, N> {
  return aabb<T, N>{pt, pt};
}

/// @ingroup geometry
/// @brief Construct an AABB from a single point (view version).
///
/// This overload accepts a `vector_view` and produces an AABB with identical
/// `min` and `max` bounds equal to the input view.
///
/// @tparam T The scalar type (e.g., float or double).
/// @tparam N The number of dimensions.
/// @param pt The input point view.
/// @return An axis-aligned bounding box with zero extent at the point.
template <typename T, std::size_t N>
auto aabb_from(const vector_view<T, N> &pt) -> aabb<T, N> {
  return aabb<T, N>{pt, pt};
}

} // namespace tf
