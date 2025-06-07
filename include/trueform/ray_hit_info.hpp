/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./intersect_status.hpp"
#include "./vector.hpp"

namespace tf {

/// @ingroup geometry
/// @brief Detailed result of a ray casting query including the intersection
/// point.
///
/// This structure provides full hit information: intersection status,
/// parametric hit distance `t`, and the corresponding point on the ray. This is
/// a heavier alternative to @ref tf::ray_cast_info when the intersection point
/// is needed.
///
/// @tparam RealT The scalar type used for the parametric distance and
/// coordinates.
/// @tparam Dims The dimensionality of the space (e.g., 2 for 2D, 3 for 3D).
template <typename RealT, std::size_t Dims> struct ray_hit_info {
  using real_t = RealT;
  /// Status of the ray cast (e.g., intersection, none, parallel, error).
  tf::intersect_status status = tf::intersect_status::none;

  /// Parametric distance `t` along the ray direction at which the intersection
  /// occurs. Meaningful only if `status == intersect_status::intersection`.
  RealT t;

  /// The intersection point on the ray. Only valid if `status == intersection`.
  tf::vector<RealT, Dims> point;

  /// @brief Checks if the ray intersects the object.
  /// @return `true` if `status == intersect_status::intersection`, `false`
  /// otherwise.
  operator bool() const { return status == tf::intersect_status::intersection; }
};

/// @ingroup geometry
/// @brief Helper to construct a @ref tf::ray_hit_info result.
///
/// @tparam RealT The scalar type used for the parametric distance and
/// coordinates.
/// @tparam Dims The dimensionality of the space.
/// @param status The intersection status.
/// @param t The parametric distance along the ray.
/// @param pt The intersection point on the ray.
/// @return A `ray_hit_info<RealT, Dims>` instance.
template <typename RealT, std::size_t Dims>
auto make_ray_hit_info(tf::intersect_status status, RealT t,
                       tf::vector<RealT, Dims> pt) {
  return ray_hit_info<RealT, Dims>{status, t, pt};
}

} // namespace tf
