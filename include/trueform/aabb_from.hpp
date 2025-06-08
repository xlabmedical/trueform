/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./aabb.hpp"
#include "./aabb_union.hpp"
#include "./polygon.hpp"
#include "./segment.hpp"
#include "./value_type.hpp"
#include "./vector_like.hpp"

namespace tf {

/// @ingroup geometry
/// @brief Construct an AABB from an aabb
///
/// The resulting AABB has identical `min` and `max` bounds equal to the input.
///
/// @tparam T The vector policy
/// @tparam N The number of dimensions (e.g., 2 or 3).
/// @param pt The input point.
/// @return An axis-aligned bounding box
template <typename RealT, std::size_t N>
auto aabb_from(const aabb<RealT, N> &box) -> aabb<RealT, N> {
  return box;
}

/// @ingroup geometry
/// @brief Construct an AABB from a single point.
///
/// The resulting AABB has identical `min` and `max` bounds equal to the input
/// point.
///
/// @tparam T The vector policy
/// @tparam N The number of dimensions (e.g., 2 or 3).
/// @param pt The input point.
/// @return An axis-aligned bounding box with zero extent at the point.
template <std::size_t N, typename T>
auto aabb_from(const vector_like<N, T> &pt) -> aabb<tf::value_type<T>, N> {
  return aabb<tf::value_type<T>, N>{pt, pt};
}

/// @ingroup geometry
/// @brief Constructs an axis-aligned bounding box (AABB) from a polygon.
///
/// This overload computes the @ref tf::aabb that tightly encloses all points in
/// the input @ref tf::polygon. The bounding box is computed by iteratively
/// expanding the initial AABB (from the first vertex) to include each
/// subsequent point.
///
/// @tparam V The number of vertices in the polygon (may be `tf::dynamic_size`).
/// @tparam Policy The underlying point access policy.
/// @param poly The input polygon.
/// @return The minimal @ref tf::aabb that contains all vertices of the polygon.
template <std::size_t V, typename Policy>
auto aabb_from(const polygon<V, Policy> &poly) {
  auto out = aabb_from(poly[0]);
  for (std::size_t i = 1; i < std::size_t(poly.size()); ++i)
    aabb_union_inplace(out, poly[i]);
  return out;
}

/// @ingroup geometry
/// @brief Constructs an axis-aligned bounding box (AABB) from a segment.
///
/// This overload computes the @ref tf::aabb that tightly encloses the two
/// endpoints of the given @ref tf::segment. The result is the minimal bounding
/// box containing both points.
///
/// @tparam Policy The underlying point access policy.
/// @param s The input segment.
/// @return The minimal @ref tf::aabb enclosing both endpoints of the segment.
template <typename Policy> auto aabb_from(const segment<Policy> &s) {
  return aabb_union(aabb_from(s[0]), s[1]);
}

} // namespace tf
