/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./aabb.hpp"
#include <cmath>

namespace tf {

/// @ingroup geometry
/// @brief Computes the squared maximal distance between two corners of two AABBs.
///
/// This function returns the maximum squared Euclidean distance between the
/// corresponding corners (`min` and `max`) of two axis-aligned bounding boxes (AABBs).
/// It is useful as a cheap, conservative approximation of the furthest possible
/// distance between points inside the boxes.
///
/// Specifically, it computes:
/// \code
/// max(length2(a.min - b.min), length2(a.max - b.max))
/// \endcode
///
/// @tparam T Numeric type.
/// @tparam N Dimensionality of the AABB (e.g., 2D, 3D).
/// @param a First AABB.
/// @param b Second AABB.
/// @return Maximum squared distance between corresponding corners of `a` and `b`.
template <typename T, std::size_t N>
auto minimal_maximal_distance2(const aabb<T, N> &a,
                               const aabb<T, N> &b) -> T {
  return std::max((a.min - b.min).length2(), (a.max - b.max).length2());
}

/// @ingroup geometry
/// @brief Computes the maximal distance between two corners of two AABBs.
///
/// This function returns the square root of the result of
/// @ref minimal_maximal_distance2. It is an upper bound approximation of the
/// maximal distance between any points inside AABBs `a` and `b`.
///
/// @tparam T Numeric type.
/// @tparam N Dimensionality of the AABB (e.g., 2D, 3D).
/// @param a First AABB.
/// @param b Second AABB.
/// @return Maximum Euclidean distance between corners of `a` and `b`.
template <typename T, std::size_t N>
auto minimal_maximal_distance(const aabb<T, N> &a,
                              const aabb<T, N> &b) -> T {
  return std::sqrt(minimal_maximal_distance2(a, b));
}

} // namespace tf
