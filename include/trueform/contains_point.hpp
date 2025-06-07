/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./contains_coplanar_point.hpp"
#include "./polygon.hpp"
#include "./projector.hpp"
#include "./static_size.hpp"
#include "./vector_like.hpp"
#include <cstddef>

namespace tf {
/// @ingroup geometry
/// @brief Checks whether a point lies inside a 2D polygon.
/// @tparam V Number of vertices in the polygon.
/// @tparam Policy Polygon storage policy.
/// @tparam T Coordinate type of the point.
/// @param poly The 2D polygon.
/// @param input_pt The 2D point to test.
/// @return True if the point lies inside the polygon.
template <std::size_t V, typename Policy, typename T>
auto contains_point(const tf::polygon<V, Policy> &poly,
                    const vector_like<2, T> &input_pt) -> bool {
  static_assert(tf::static_size_v<decltype(poly[0])> == 2,
                "Points must be in 2D.");
  return contains_coplanar_point(poly, input_pt);
}
/// @ingroup geometry
/// @brief Checks whether a point lies inside a 3D polygon by projecting to 2D.
/// @tparam V Number of vertices in the polygon.
/// @tparam Policy Polygon storage policy.
/// @tparam Dims Spatial dimensions.
/// @tparam T Coordinate type of the point.
/// @param poly The polygon.
/// @param input_pt The point to test.
/// @return True if the point lies inside the polygon.
template <std::size_t V, typename Policy, std::size_t Dims, typename T>
auto contains_point(const tf::polygon<V, Policy> &poly_in,
                    const vector_like<Dims, T> &input_pt) -> bool {
  const auto &poly = tf::inject_plane(poly_in);
  auto d = tf::dot(poly.plane().normal, input_pt) + poly.plane().d;
  if (std::abs(d) > std::numeric_limits<decltype(d)>::epsilon())
    return false;
  return contains_coplanar_point(poly, input_pt - d * poly.plane().normal,
                                 tf::make_simple_projector(poly.plane().normal),
                                 std::numeric_limits<decltype(d)>::epsilon());
}
} // namespace tf
