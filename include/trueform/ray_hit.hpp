/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./ray_cast.hpp"
#include "./ray_hit_info.hpp"

namespace tf {

/// @ingroup geometry
/// @brief Computes the intersection of a ray with a plane.
///
/// This function wraps `ray_cast` and computes the actual intersection point
/// (if any) between the ray and the plane. The result includes the hit status,
/// intersection parameter `t`, and the world-space intersection point.
///
/// @tparam RealT The scalar type (e.g., float or double).
/// @tparam Dims The number of spatial dimensions.
/// @param ray The ray to test.
/// @param plane The plane to test against.
/// @param config Configuration parameters for ray casting, such as epsilon and
/// t-min/t-max.
/// @return A `ray_hit_info` struct containing the status, parameter `t`, and
/// hit point (if any).
template <typename RealT, std::size_t Dims>
auto ray_hit(const ray<RealT, Dims> &ray, const tf::plane<RealT, Dims> &plane,
             const tf::ray_config<RealT> &config = tf::ray_config<RealT>{}) {
  auto result = ray_cast(ray, plane, config);
  tf::ray_hit_info<RealT, Dims> out;
  out.status = result.status;
  out.t = result.t;
  if (result)
    out.point = ray.origin + result.t * ray.direction;
  return out;
}

/// @ingroup geometry
/// @brief Computes the intersection of a ray with a polygon (lying on a plane).
///
/// This function wraps `ray_cast` and additionally checks if the intersection
/// point lies inside the polygon. If so, the hit is considered valid and the
/// result includes the hit point and updated status. Otherwise, the hit is
/// marked as a miss.
///
/// @tparam RealT The scalar type (e.g., float or double).
/// @tparam Dims The number of spatial dimensions.
/// @tparam V The maximum vertex count of the polygon.
/// @tparam Policy The policy type used by the polygon (e.g., owning or
/// referencing).
/// @param ray The ray to test.
/// @param poly The polygon to test against.
/// @param config Configuration parameters for ray casting.
/// @return A `ray_hit_info` struct containing status, parameter `t`, and hit
/// point (if any).
template <typename RealT, std::size_t Dims, std::size_t V, typename Policy>
auto ray_hit(const ray<RealT, Dims> &ray, const tf::polygon<V, Policy> &poly_in,
             const tf::ray_config<RealT> &config = tf::ray_config<RealT>{}) {
  const auto &poly = tf::inject_plane(poly_in);
  auto result = ray_cast(ray, poly.plane(), config);
  tf::ray_hit_info<RealT, Dims> out;
  out.status = result.status;
  out.t = result.t;
  if (result) {
    out.point = ray.origin + result.t * ray.direction;
    out.status = static_cast<tf::intersect_status>(tf::contains_coplanar_point(
        poly, out.point, tf::make_simple_projector(poly.normal()),
        std::numeric_limits<RealT>::epsilon()));
  }
  return out;
}

} // namespace tf
