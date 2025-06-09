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

template <typename RealT, std::size_t Dims, typename Policy>
auto ray_hit(const ray<RealT, Dims> &ray, const tf::segment<Policy> &seg,
             const tf::ray_config<RealT> &config = tf::ray_config<RealT>{}) {
  auto ray1 = tf::make_ray_between_points(seg[0], seg[1]);
  auto [non_parallel, t0, t1] = tf::implementation::line_line_check(ray, ray1);
  intersect_status status = intersect_status::none;
  tf::vector<tf::common_value<decltype(t0), decltype(t1)>, Dims> pt;
  if (non_parallel && t0 >= config.min_t && t0 <= config.max_t && t1 >= 0 &&
      t1 <= 1) {
    auto pt0 = ray.origin + t0 * ray.direction;
    auto pt1 = ray1.origin + t1 * ray1.direction;
    auto d2 = (pt0 - pt1).length2();
    status = static_cast<intersect_status>(
        d2 < std::numeric_limits<decltype(d2)>::epsilon());
    pt = (pt0 + pt1) / 2;
  }
  return tf::make_ray_hit_info(status, t0, pt);
}

/// @brief Performs a ray cast against a spatial tree and returns intersection
/// information.
///
/// This function traverses a spatial acceleration structure (such as a BVH or
/// custom tree) to find the closest intersected primitive along the given ray.
/// It uses a user-provided intersection function `ray_cast_f` to test
/// ray-primitive intersections at the leaves.
///
/// The search is bounded by the optional ray interval defined in `config`,
/// which allows pruning of far-away intersections or early-out behavior. The
/// result contains both the intersected element index and additional
/// intersection details (e.g., hit distance, position).
///
/// @tparam RealT Floating-point type (e.g., `float`, `double`) used for ray and
/// bounding box coordinates.
/// @tparam Dims Dimensionality of the ray and tree (typically 2 or 3).
/// @tparam Index Index type used to identify primitives (e.g., `std::size_t`,
/// `int32_t`).
/// @tparam F Callable type used for ray-primitive intersection testing. Must
/// return a
///         `tf::ray_hit_info<RealT, Dims>`  if a hit occurs.
///
/// @param ray The ray to cast into the tree.
/// @param tree The spatial tree containing the primitives to test.
/// @param ray_cast_f A callable that performs ray-primitive intersection tests
/// at the leaf level.
///                   Called as `ray_cast_f(ray, index)` and must return a valid
///                   hit result or a sentinel.
/// @param config Optional configuration for the ray cast, including `min_t` and
/// `max_t` bounds on the ray interval.
///
/// @return A `tf::tree_ray_info<Index, tf::ray_hit_info<RealT, Dims>>`
/// containing the index of the hit element
///         and the associated hit data. If no hit occurs, the result evaluates
///         to `false`.
template <typename RealT, std::size_t Dims, typename Index, typename F>
auto ray_hit(const ray<RealT, Dims> &ray,
             const tf::tree<Index, RealT, Dims> &tree, const F &ray_hit_f,
             const tf::ray_config<RealT> &config = {}) {
  tf::implementation::tree_ray_info<
      Index, tf::tree_ray_info<Index, tf::ray_hit_info<RealT, Dims>>>
      result{config.min_t, config.max_t};
  tf::implementation::tree_ray_cast(tree, ray, result, ray_hit_f);
  return result.info();
}

/// @brief Performs a ray cast against a spatial tree and returns intersection
/// information.
///
/// This function traverses a spatial acceleration structure (such as a BVH or
/// custom tree) to find the closest intersected primitive along the given ray.
/// It uses a user-provided intersection function `ray_cast_f` to test
/// ray-primitive intersections at the leaves.
///
/// The search is bounded by the optional ray interval defined in `config`,
/// which allows pruning of far-away intersections or early-out behavior. The
/// result contains both the intersected element index and additional
/// intersection details (e.g., hit distance, position).
///
/// @tparam RealT Floating-point type (e.g., `float`, `double`) used for ray and
/// bounding box coordinates.
/// @tparam Dims Dimensionality of the ray and tree (typically 2 or 3).
/// @tparam Index Index type used to identify primitives (e.g., `std::size_t`,
/// `int32_t`).
/// @tparam F Callable type used for ray-primitive intersection testing. Must
/// return a
///         `tf::ray_hit_info<RealT, Dims>`  if a hit occurs.
///
/// @param ray The ray to cast into the tree.
/// @param tree The spatial tree containing the primitives to test.
/// @param ray_cast_f A callable that performs ray-primitive intersection tests
/// at the leaf level.
///                   Called as `ray_cast_f(ray, index)` and must return a valid
///                   hit result or a sentinel.
/// @param config Optional configuration for the ray cast, including `min_t` and
/// `max_t` bounds on the ray interval.
///
/// @return A `tf::tree_ray_info<Index, tf::ray_hit_info<RealT, Dims>>`
/// containing the index of the hit element
///         and the associated hit data. If no hit occurs, the result evaluates
///         to `false`.
template <typename RealT, std::size_t Dims, typename Index, typename F>
auto ray_hit(const ray<RealT, Dims> &ray,
             const tf::mod_tree<Index, RealT, Dims> &tree, const F &ray_cast_f,
             tf::ray_config<RealT> config = {}) {
  tf::implementation::tree_ray_info<
      Index, tf::tree_ray_info<Index, tf::ray_hit_info<RealT, Dims>>>
      result{config.min_t, config.max_t};
  tf::implementation::tree_ray_cast(tree.main_tree(), ray, result, ray_cast_f);
  tf::implementation::tree_ray_cast(tree.delta_tree(), ray, result, ray_cast_f);
  return result.info();
}

} // namespace tf
