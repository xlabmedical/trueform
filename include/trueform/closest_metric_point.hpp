/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./closest_point_parametric.hpp"
#include "./metric_point.hpp"

namespace tf {
/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <std::size_t Dims, typename T0, typename T1>
auto closest_metric_point(const tf::vector_like<Dims, T0> &v0,
                          const tf::vector_like<Dims, T1> &v1) {
  return tf::make_metric_point((v0 - v1).length2(), v0);
}
/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <typename RealT, std::size_t Dims, typename T1>
auto closest_metric_point(const tf::line<RealT, Dims> &l,
                          const tf::vector_like<Dims, T1> &v1) {
  auto t = tf::closest_point_parametric(l, v1);
  auto pt = l.origin + t * l.direction;
  return tf::make_metric_point((pt - v1).length2(), pt);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <std::size_t Dims, typename T1, typename RealT>
auto closest_metric_point(const tf::vector_like<Dims, T1> &v0,
                          const tf::line<RealT, Dims> &l) {
  auto t = tf::closest_point_parametric(l, v0);
  auto pt = l.origin + t * l.direction;
  return tf::make_metric_point((pt - v0).length2(), v0);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <typename RealT, std::size_t Dims, typename T1>
auto closest_metric_point(const tf::ray<RealT, Dims> &r,
                          const tf::vector_like<Dims, T1> &v1) {
  auto t = tf::closest_point_parametric(r, v1);
  auto pt = r.origin + t * r.direction;
  return tf::make_metric_point((pt - v1).length2(), pt);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <std::size_t Dims, typename T1, typename RealT>
auto closest_metric_point(const tf::vector_like<Dims, T1> &v0,
                          const tf::ray<RealT, Dims> &r) {
  auto t = tf::closest_point_parametric(r, v0);
  auto pt = r.origin + t * r.direction;
  return tf::make_metric_point((pt - v0).length2(), v0);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <typename T0, std::size_t Dims, typename T1>
auto closest_metric_point(const tf::segment<T0> &s,
                          const tf::vector_like<Dims, T1> &v1) {
  auto t = tf::closest_point_parametric(s, v1);
  auto l = tf::make_line_between_points(s[0], s[1]);
  auto pt = l.origin + t * l.direction;
  return tf::make_metric_point((pt - v1).length2(), pt);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <std::size_t Dims, typename T0, typename T1>
auto closest_metric_point(const tf::vector_like<Dims, T0> &v0,
                          const tf::segment<T1> &s) {
  auto t = tf::closest_point_parametric(s, v0);
  auto l = tf::make_line_between_points(s[0], s[1]);
  auto pt = l.origin + t * l.direction;
  return tf::make_metric_point((pt - v0).length2(), v0);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <typename RealT, std::size_t Dims>
auto closest_metric_point(const tf::line<RealT, Dims> &l0,
                          const tf::line<RealT, Dims> &l1) {
  auto [t0, t1] = tf::closest_point_parametric(l0, l1);
  auto pt0 = l0.origin + t0 * l0.direction;
  auto pt1 = l1.origin + t1 * l1.direction;
  return tf::make_metric_point((pt0 - pt1).length2(), pt0);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <typename RealT, std::size_t Dims>
auto closest_metric_point(const tf::ray<RealT, Dims> &r0,
                          const tf::ray<RealT, Dims> &r1) {
  auto [t0, t1] = tf::closest_point_parametric(r0, r1);
  auto pt0 = r0.origin + t0 * r0.direction;
  auto pt1 = r1.origin + t1 * r1.direction;
  return tf::make_metric_point((pt0 - pt1).length2(), pt0);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <typename RealT, std::size_t Dims>
auto closest_metric_point(const tf::line<RealT, Dims> &l0,
                          const tf::ray<RealT, Dims> &r1) {
  auto [t0, t1] = tf::closest_point_parametric(l0, r1);
  auto pt0 = l0.origin + t0 * l0.direction;
  auto pt1 = r1.origin + t1 * r1.direction;
  return tf::make_metric_point((pt0 - pt1).length2(), pt0);
}
/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <typename RealT, std::size_t Dims>
auto closest_metric_point(const tf::ray<RealT, Dims> &r0,
                          const tf::line<RealT, Dims> &l1) {
  auto [t0, t1] = tf::closest_point_parametric(r0, l1);
  auto pt0 = r0.origin + t0 * r0.direction;
  auto pt1 = l1.origin + t1 * l1.direction;
  return tf::make_metric_point((pt0 - pt1).length2(), pt0);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <typename RealT, std::size_t Dims, typename T>
auto closest_metric_point(const tf::ray<RealT, Dims> &r0,
                          const tf::segment<T> &s1) {
  auto l1 = tf::make_line_between_points(s1[0], s1[1]);
  auto [t0, t1] = tf::closest_point_parametric(r0, l1);
  auto pt0 = r0.origin + t0 * r0.direction;
  auto pt1 = l1.origin + t1 * l1.direction;
  return tf::make_metric_point((pt0 - pt1).length2(), pt0);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <typename RealT, std::size_t Dims, typename T>
auto closest_metric_point(const tf::line<RealT, Dims> &l0,
                          const tf::segment<T> &s1) {
  auto l1 = tf::make_line_between_points(s1[0], s1[1]);
  auto [t0, t1] = tf::closest_point_parametric(l0, l1);
  auto pt0 = l0.origin + t0 * l0.direction;
  auto pt1 = l1.origin + t1 * l1.direction;
  return tf::make_metric_point((pt0 - pt1).length2(), pt0);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <typename T, std::size_t Dims, typename RealT>
auto closest_metric_point(const tf::segment<T> &s0,
                          const tf::line<RealT, Dims> &l1) {
  auto l0 = tf::make_line_between_points(s0[0], s0[1]);
  auto [t0, t1] = tf::closest_point_parametric(l0, l1);
  auto pt0 = l0.origin + t0 * l0.direction;
  auto pt1 = l1.origin + t1 * l1.direction;
  return tf::make_metric_point((pt0 - pt1).length2(), pt0);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <typename T, std::size_t Dims, typename RealT>
auto closest_metric_point(const tf::segment<T> &s0,
                          const tf::ray<RealT, Dims> &r1) {
  auto l0 = tf::make_line_between_points(s0[0], s0[1]);
  auto [t0, t1] = tf::closest_point_parametric(l0, r1);
  auto pt0 = l0.origin + t0 * l0.direction;
  auto pt1 = r1.origin + t1 * r1.direction;
  return tf::make_metric_point((pt0 - pt1).length2(), pt0);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point on the first object.
template <typename T0, typename T1>
auto closest_metric_point(const tf::segment<T0> &s0,
                          const tf::segment<T1> &s1) {
  auto l0 = tf::make_line_between_points(s0[0], s0[1]);
  auto l1 = tf::make_line_between_points(s1[0], s1[1]);
  auto [t0, t1] = tf::closest_point_parametric(l0, l1);
  auto pt0 = l0.origin + t0 * l0.direction;
  auto pt1 = l1.origin + t1 * l1.direction;
  return tf::make_metric_point((pt0 - pt1).length2(), pt0);
}


} // namespace tf
