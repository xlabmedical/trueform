/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./contains_coplanar_point.hpp"
#include "./dot.hpp"
#include "./plane.hpp"
#include "./polygon.hpp"
#include "./ray.hpp"
#include "./ray_cast_info.hpp"
#include "./ray_config.hpp"
#include <limits>

namespace tf {
/// @ingroup geometry
/// @brief Casts a ray against a plane and computes the intersection parameter.
///
/// Determines whether a ray intersects a plane and, if so, computes the
/// intersection parameter `t` along the ray. The result is filtered by the
/// configured bounds `min_t` and `max_t`. If the ray is parallel to the plane,
/// the status is `intersect_status::parallel`. If the ray origin lies on the
/// plane, the status is `intersect_status::error`.
///
/// @tparam RealT The scalar type (e.g., float or double).
/// @tparam Dims The number of dimensions (typically 3).
/// @param ray The ray to be cast.
/// @param plane The target plane.
/// @param config Configuration specifying `[min_t, max_t]` bounds to accept
/// valid intersections.
/// @return A `ray_cast_info<RealT>` containing the intersection `status` and
/// parameter `t`.
template <typename RealT, std::size_t Dims>
auto ray_cast(const ray<RealT, Dims> &ray, const tf::plane<RealT, Dims> &plane,
              const tf::ray_config<RealT> &config = tf::ray_config<RealT>{}) {
  auto Vd = tf::dot(plane.normal, ray.direction);
  auto V0 = tf::dot(plane.normal, ray.origin) + plane.d;
  RealT t = 0;
  if (std::abs(Vd) < std::numeric_limits<RealT>::epsilon()) {
    if (std::abs(V0) < std::numeric_limits<RealT>::epsilon()) {
      return tf::make_ray_cast_info(tf::intersect_status::error, t);
    } else {
      return tf::make_ray_cast_info(tf::intersect_status::parallel, t);
    }
  }

  t = -V0 / Vd;
  return tf::make_ray_cast_info(
      static_cast<tf::intersect_status>(char(t >= config.min_t) &
                                        char(t <= config.max_t)),
      t);
}
/// @ingroup geometry
/// @brief Casts a ray against a convex polygon and tests for intersection.
///
/// First computes the intersection between the ray and the polygon's supporting
/// plane. If a valid intersection `t` is found within `[min_t, max_t]`, it
/// further checks whether the intersection point lies inside the polygon using
/// a coplanar point-in-polygon test.
///
/// @tparam RealT The scalar type.
/// @tparam Dims The number of dimensions.
/// @tparam V The compile-time maximum number of polygon vertices.
/// @tparam Policy The policy used to store polygon data.
/// @param ray The ray to cast.
/// @param poly The convex polygon to test against.
/// @param config Configuration specifying `[min_t, max_t]` bounds to accept
/// valid intersections.
/// @return A `ray_cast_info<RealT>` containing the intersection `status` and
/// parameter `t`.
template <typename RealT, std::size_t Dims, std::size_t V, typename Policy>
auto ray_cast(const ray<RealT, Dims> &ray, const tf::polygon<V, Policy> &poly_in,
              const tf::ray_config<RealT> &config = tf::ray_config<RealT>{}) {
  const auto &poly = tf::inject_plane(poly_in);
  auto result = ray_cast(ray, poly.plane(), config);
  if (result) {
    auto pt = ray.origin + result.t * ray.direction;
    result.status =
        static_cast<tf::intersect_status>(tf::contains_coplanar_point(
            poly, pt, tf::make_simple_projector(poly.normal()),
            std::numeric_limits<RealT>::epsilon()));
  }
  return result;
}

} // namespace tf
