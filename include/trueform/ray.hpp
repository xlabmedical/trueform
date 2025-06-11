/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./value_type.hpp"
#include "./point.hpp"
#include "./vector.hpp"

namespace tf {

/// @ingroup geometry
/// @brief Represents a ray in D-dimensional space.
///
/// A ray consists of an origin point and a direction vector. The direction
/// vector is not necessarily normalized.
///
/// @tparam T The scalar type (e.g., float, double).
/// @tparam Dims The number of spatial dimensions.
template <typename T, std::size_t Dims> struct ray {
  tf::point<T, Dims> origin;    ///< The origin point of the ray.
  tf::vector<T, Dims> direction; ///< The direction vector of the ray.
};

/// @ingroup geometry
/// @brief Constructs a ray from an origin and a direction vector.
///
/// The direction does not need to be normalized. The resulting ray's
/// value type is the common type between the two vector components.
///
/// @tparam T0 The type of the origin vector's components.
/// @tparam T1 The type of the direction vector's components.
/// @tparam Dims The spatial dimension of the vectors.
/// @param origin The starting point of the ray.
/// @param direction The direction vector of the ray.
/// @return A ray object.
template <typename T0, std::size_t Dims, typename T1>
auto make_ray(const tf::point_like<Dims, T0> &origin,
              const tf::vector_like<Dims, T1> &direction) {
  return ray<tf::common_value<T0, T1>, Dims>{origin, direction};
}

/// @ingroup geometry
/// @brief Constructs a ray from two points, with the direction pointing from
/// origin to end.
///
/// The direction is computed as `end - origin`. The resulting ray's
/// value type is the common type between the two points.
///
/// @tparam T0 The type of the origin point.
/// @tparam T1 The type of the end point.
/// @tparam Dims The spatial dimension of the points.
/// @param origin The starting point of the ray.
/// @param end The end point; the direction of the ray will point from `origin`
/// to `end`.
/// @return A ray object.
template <typename T0, std::size_t Dims, typename T1>
auto make_ray_between_points(const tf::point_like<Dims, T0> &origin,
                             const tf::point_like<Dims, T1> &end) {
  return ray<tf::common_value<T0, T1>, Dims>{origin, end - origin};
}

} // namespace tf
