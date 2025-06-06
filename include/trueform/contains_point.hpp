/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./contains_coplanar_point.hpp"
#include "./inject_normal.hpp"
#include "./inject_plane.hpp"
#include "./polygon.hpp"
#include "./projector.hpp"
#include "./static_size.hpp"
#include "./vector_like.hpp"
#include <cstddef>
#include <limits>

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
/// @brief Checks whether a point lies inside a polygon with plane injection.
/// @tparam RealT Scalar type.
/// @tparam Dims Spatial dimensions.
/// @tparam Policy Polygon storage policy.
/// @tparam T Coordinate type of the point.
/// @param poly The polygon with a plane.
/// @param input_pt The point to test.
/// @return True if the point lies inside the polygon.
template <typename RealT, std::size_t Dims, typename Policy, typename T>
auto contains_point(const tf::inject_plane_t<RealT, Dims, Policy> &poly,
                    const vector_like<Dims, T> &input_pt) -> bool {
  auto d = tf::dot(poly.plane().normal, input_pt) + poly.plane().d;
  if (std::abs(d) > std::numeric_limits<decltype(d)>::epsilon())
    return false;
  return contains_coplanar_point(poly, input_pt - d * poly.normal(),
                        tf::make_simple_projector(poly.normal()),
                        std::numeric_limits<decltype(d)>::epsilon());
}

/// @ingroup geometry
/// @brief Checks whether a point lies inside a polygon with normal injection.
/// @tparam RealT Scalar type.
/// @tparam Dims Spatial dimensions.
/// @tparam Policy Polygon storage policy.
/// @tparam T Coordinate type of the point.
/// @param poly The polygon with a normal.
/// @param input_pt The point to test.
/// @return True if the point lies inside the polygon.
template <typename RealT, std::size_t Dims, typename Policy, typename T>
auto contains_point(const tf::inject_normal_t<RealT, Dims, Policy> &poly,
                    const vector_like<Dims, T> &input_pt) -> bool {
  auto plane = tf::make_plane(poly.normal(), poly[0]);
  auto d = tf::dot(plane.normal, input_pt) + plane.d;
  if (std::abs(d) > std::numeric_limits<decltype(d)>::epsilon())
    return false;
  return contains_coplanar_point(poly, input_pt - d * poly.normal(),
                        tf::make_simple_projector(poly.normal()),
                        std::numeric_limits<decltype(d)>::epsilon());
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
auto contains_point(const tf::polygon<V, Policy> &poly,
                    const vector_like<Dims, T> &input_pt) -> bool {
  auto plane = tf::make_plane(poly[0], poly[1], poly[2]);
  auto d = tf::dot(plane.normal, input_pt) + plane.d;
  if (std::abs(d) > std::numeric_limits<decltype(d)>::epsilon())
    return false;
  return contains_coplanar_point(poly, input_pt - d * plane.normal,
                        tf::make_simple_projector(plane.normal),
                        std::numeric_limits<decltype(d)>::epsilon());
}
} // namespace tf
