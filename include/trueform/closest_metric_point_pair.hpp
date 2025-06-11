/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./closest_point_on_triangle.hpp"
#include "./closest_point_parametric.hpp"
#include "./contains_coplanar_point.hpp"
#include "./metric_point_pair.hpp"
#include "./polygon.hpp"
#include "./ray_hit.hpp"

namespace tf {
/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <std::size_t Dims, typename T0, typename T1>
auto closest_metric_point_pair(const tf::point_like<Dims, T0> &v0,
                               const tf::point_like<Dims, T1> &v1) {
  return tf::make_metric_point_pair((v0 - v1).length2(), v0, v1);
}
/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename RealT, std::size_t Dims, typename T1>
auto closest_metric_point_pair(const tf::line<RealT, Dims> &l,
                               const tf::point_like<Dims, T1> &v1) {
  auto t = tf::closest_point_parametric(l, v1);
  auto pt = l.origin + t * l.direction;
  return tf::make_metric_point_pair((pt - v1).length2(), pt, v1);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <std::size_t Dims, typename T1, typename RealT>
auto closest_metric_point_pair(const tf::point_like<Dims, T1> &v0,
                               const tf::line<RealT, Dims> &l) {
  auto t = tf::closest_point_parametric(l, v0);
  auto pt = l.origin + t * l.direction;
  return tf::make_metric_point_pair((pt - v0).length2(), v0, pt);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename RealT, std::size_t Dims, typename T1>
auto closest_metric_point_pair(const tf::ray<RealT, Dims> &r,
                               const tf::point_like<Dims, T1> &v1) {
  auto t = tf::closest_point_parametric(r, v1);
  auto pt = r.origin + t * r.direction;
  return tf::make_metric_point_pair((pt - v1).length2(), pt, v1);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <std::size_t Dims, typename T1, typename RealT>
auto closest_metric_point_pair(const tf::point_like<Dims, T1> &v0,
                               const tf::ray<RealT, Dims> &r) {
  auto t = tf::closest_point_parametric(r, v0);
  auto pt = r.origin + t * r.direction;
  return tf::make_metric_point_pair((pt - v0).length2(), v0, pt);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename T0, std::size_t Dims, typename T1>
auto closest_metric_point_pair(const tf::segment<T0> &s,
                               const tf::point_like<Dims, T1> &v1) {
  auto t = tf::closest_point_parametric(s, v1);
  auto l = tf::make_line_between_points(s[0], s[1]);
  auto pt = l.origin + t * l.direction;
  return tf::make_metric_point_pair((pt - v1).length2(), pt, v1);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <std::size_t Dims, typename T0, typename T1>
auto closest_metric_point_pair(const tf::point_like<Dims, T0> &v0,
                               const tf::segment<T1> &s) {
  auto t = tf::closest_point_parametric(s, v0);
  auto l = tf::make_line_between_points(s[0], s[1]);
  auto pt = l.origin + t * l.direction;
  return tf::make_metric_point_pair((pt - v0).length2(), v0, pt);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename RealT, std::size_t Dims>
auto closest_metric_point_pair(const tf::line<RealT, Dims> &l0,
                               const tf::line<RealT, Dims> &l1) {
  auto [t0, t1] = tf::closest_point_parametric(l0, l1);
  auto pt0 = l0.origin + t0 * l0.direction;
  auto pt1 = l1.origin + t1 * l1.direction;
  return tf::make_metric_point_pair((pt0 - pt1).length2(), pt0, pt1);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename RealT, std::size_t Dims>
auto closest_metric_point_pair(const tf::ray<RealT, Dims> &r0,
                               const tf::ray<RealT, Dims> &r1) {
  auto [t0, t1] = tf::closest_point_parametric(r0, r1);
  auto pt0 = r0.origin + t0 * r0.direction;
  auto pt1 = r1.origin + t1 * r1.direction;
  return tf::make_metric_point_pair((pt0 - pt1).length2(), pt0, pt1);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename RealT, std::size_t Dims>
auto closest_metric_point_pair(const tf::line<RealT, Dims> &l0,
                               const tf::ray<RealT, Dims> &r1) {
  auto [t0, t1] = tf::closest_point_parametric(l0, r1);
  auto pt0 = l0.origin + t0 * l0.direction;
  auto pt1 = r1.origin + t1 * r1.direction;
  return tf::make_metric_point_pair((pt0 - pt1).length2(), pt0, pt1);
}
/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename RealT, std::size_t Dims>
auto closest_metric_point_pair(const tf::ray<RealT, Dims> &r0,
                               const tf::line<RealT, Dims> &l1) {
  auto [t0, t1] = tf::closest_point_parametric(r0, l1);
  auto pt0 = r0.origin + t0 * r0.direction;
  auto pt1 = l1.origin + t1 * l1.direction;
  return tf::make_metric_point_pair((pt0 - pt1).length2(), pt0, pt1);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename RealT, std::size_t Dims, typename T>
auto closest_metric_point_pair(const tf::ray<RealT, Dims> &r0,
                               const tf::segment<T> &s1) {
  auto l1 = tf::make_line_between_points(s1[0], s1[1]);
  auto [t0, t1] = tf::closest_point_parametric(r0, l1);
  auto pt0 = r0.origin + t0 * r0.direction;
  auto pt1 = l1.origin + t1 * l1.direction;
  return tf::make_metric_point_pair((pt0 - pt1).length2(), pt0, pt1);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename RealT, std::size_t Dims, typename T>
auto closest_metric_point_pair(const tf::line<RealT, Dims> &l0,
                               const tf::segment<T> &s1) {
  auto l1 = tf::make_line_between_points(s1[0], s1[1]);
  auto [t0, t1] = tf::closest_point_parametric(l0, l1);
  auto pt0 = l0.origin + t0 * l0.direction;
  auto pt1 = l1.origin + t1 * l1.direction;
  return tf::make_metric_point_pair((pt0 - pt1).length2(), pt0, pt1);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename T, std::size_t Dims, typename RealT>
auto closest_metric_point_pair(const tf::segment<T> &s0,
                               const tf::line<RealT, Dims> &l1) {
  auto l0 = tf::make_line_between_points(s0[0], s0[1]);
  auto [t0, t1] = tf::closest_point_parametric(l0, l1);
  auto pt0 = l0.origin + t0 * l0.direction;
  auto pt1 = l1.origin + t1 * l1.direction;
  return tf::make_metric_point_pair((pt0 - pt1).length2(), pt0, pt1);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename T, std::size_t Dims, typename RealT>
auto closest_metric_point_pair(const tf::segment<T> &s0,
                               const tf::ray<RealT, Dims> &r1) {
  auto l0 = tf::make_line_between_points(s0[0], s0[1]);
  auto [t0, t1] = tf::closest_point_parametric(l0, r1);
  auto pt0 = l0.origin + t0 * l0.direction;
  auto pt1 = r1.origin + t1 * r1.direction;
  return tf::make_metric_point_pair((pt0 - pt1).length2(), pt0, pt1);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename T0, typename T1>
auto closest_metric_point_pair(const tf::segment<T0> &s0,
                               const tf::segment<T1> &s1) {
  auto l0 = tf::make_line_between_points(s0[0], s0[1]);
  auto l1 = tf::make_line_between_points(s1[0], s1[1]);
  auto [t0, t1] = tf::closest_point_parametric(l0, l1);
  auto pt0 = l0.origin + t0 * l0.direction;
  auto pt1 = l1.origin + t1 * l1.direction;
  return tf::make_metric_point_pair((pt0 - pt1).length2(), pt0, pt1);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename Policy0, std::size_t Dims, typename Policy1>
auto closest_metric_point_pair(const tf::polygon<3, Policy0> &poly,
                               const tf::point_like<Dims, Policy1> &pt) {
  auto c_pt = tf::closest_point_on_triangle(poly, pt);
  return tf::make_metric_point_pair((c_pt - pt).length2(), c_pt, pt);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <std::size_t Dims, typename Policy1, typename Policy0>
auto closest_metric_point_pair(const tf::point_like<Dims, Policy1> &pt,
                               const tf::polygon<3, Policy0> &poly) {
  auto res = closest_metric_point_pair(poly, pt);
  std::swap(res.first, res.second);
  return res;
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <std::size_t V, typename Policy0, std::size_t Dims, typename Policy1>
auto closest_metric_point_pair(const tf::polygon<V, Policy0> &poly_in,
                               const tf::point_like<Dims, Policy1> &pt) {
  const auto &poly = tf::inject_plane(poly_in);
  auto d = tf::dot(poly.plane().normal, pt) + poly.plane().d;
  auto c_pt = pt - d * poly.plane().normal;
  if (std::abs(d) < std::numeric_limits<decltype(d)>::epsilon() &&
      tf::contains_coplanar_point(poly, c_pt))
    return tf::make_metric_point_pair(d * d, c_pt, pt);
  std::size_t size = poly.size();
  std::size_t prev = size - 1;
  auto d2 = d * d;
  for (std::size_t i = 0; i < size; prev = i++) {
    auto line = tf::make_line_between_points(poly[prev], poly[i]);
    auto t = tf::closest_point_parametric(line, pt);
    t = std::clamp(t, decltype(t)(0), decltype(t)(1));
    auto tmp_c_pt = line.origin + t * line.direction;
    auto tmp_d2 = (tmp_c_pt - pt).length2();
    if (tmp_d2 < d2) {
      d2 = tmp_d2;
      c_pt = tmp_c_pt;
    }
  }
  return tf::make_metric_point_pair(d2, c_pt, pt);
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <std::size_t Dims, typename Policy1, std::size_t V, typename Policy0>
auto closest_metric_point_pair(const tf::point_like<Dims, Policy1> &pt,
                               const tf::polygon<V, Policy0> &poly) {
  auto res = closest_metric_point_pair(poly, pt);
  std::swap(res.first, res.second);
  return res;
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <std::size_t V, typename Policy0, typename RealT, std::size_t Dims>
auto closest_metric_point_pair(const tf::polygon<V, Policy0> &poly_in,
                               const tf::line<RealT, Dims> &line) {
  const auto &poly = tf::inject_plane(poly_in);
  auto hit_info =
      tf::ray_hit(tf::make_ray(line.origin, line.direction), poly,
                  tf::make_ray_config(-std::numeric_limits<RealT>::max(),
                                      std::numeric_limits<RealT>::max()));
  if (hit_info) {
    return tf::make_metric_point_pair(RealT(0), hit_info.point, hit_info.point);
  }
  auto best = tf::make_metric_point_pair(std::numeric_limits<RealT>::max(),
                                         hit_info.point, hit_info.point);
  std::size_t size = poly.size();
  std::size_t prev = size - 1;
  for (std::size_t i = 0; i < size; prev = i++) {
    auto seg = tf::make_segment_between_points(poly[prev], poly[i]);
    auto tmp = tf::closest_metric_point_pair(seg, line);
    if (tmp.metric < best.metric)
      best = tmp;
  }
  return best;
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename RealT, std::size_t Dims, std::size_t V, typename Policy0>
auto closest_metric_point_pair(const tf::line<RealT, Dims> &line,
                               const tf::polygon<V, Policy0> &poly) {
  auto res = closest_metric_point_pair(poly, line);
  std::swap(res.first, res.second);
  return res;
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <std::size_t V, typename Policy0, typename RealT, std::size_t Dims>
auto closest_metric_point_pair(const tf::polygon<V, Policy0> &poly_in,
                               const tf::ray<RealT, Dims> &ray) {
  const auto &poly = tf::inject_plane(poly_in);
  auto hit_info = tf::ray_hit(ray, poly);
  if (hit_info) {
    return tf::make_metric_point_pair(RealT(0), hit_info.point, hit_info.point);
  }
  auto best = tf::make_metric_point_pair(std::numeric_limits<RealT>::max(),
                                         hit_info.point, hit_info.point);
  std::size_t size = poly.size();
  std::size_t prev = size - 1;
  for (std::size_t i = 0; i < size; prev = i++) {
    auto seg = tf::make_segment_between_points(poly[prev], poly[i]);
    auto tmp = tf::closest_metric_point_pair(seg, ray);
    if (tmp.metric < best.metric)
      best = tmp;
  }
  return best;
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename RealT, std::size_t Dims, std::size_t V, typename Policy0>
auto closest_metric_point_pair(const tf::ray<RealT, Dims> &ray,
                               const tf::polygon<V, Policy0> &poly) {
  auto res = closest_metric_point_pair(poly, ray);
  std::swap(res.first, res.second);
  return res;
}
/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <std::size_t V, typename Policy0, typename Policy1>
auto closest_metric_point_pair(const tf::polygon<V, Policy0> &poly_in,
                               const tf::segment<Policy1> &seg1) {
  const auto &poly = tf::inject_plane(poly_in);
  auto ray = tf::make_ray_between_points(seg1[0], seg1[1]);
  using RealT = tf::common_value<decltype(poly[0][0]), decltype(seg1[0][0])>;
  auto hit_info =
      tf::ray_hit(ray, poly, tf::make_ray_config(RealT(0), RealT(1)));
  if (hit_info) {
    return tf::make_metric_point_pair(RealT(0), hit_info.point, hit_info.point);
  }
  auto best = tf::make_metric_point_pair(std::numeric_limits<RealT>::max(),
                                         hit_info.point, hit_info.point);
  std::size_t size = poly.size();
  std::size_t prev = size - 1;
  for (std::size_t i = 0; i < size; prev = i++) {
    auto seg = tf::make_segment_between_points(poly[prev], poly[i]);
    auto tmp = tf::closest_metric_point_pair(seg, seg1);
    if (tmp.metric < best.metric)
      best = tmp;
  }
  return best;
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <typename Policy, std::size_t V, typename Policy0>
auto closest_metric_point_pair(const tf::segment<Policy> &seg,
                               const tf::polygon<V, Policy0> &poly) {
  auto res = closest_metric_point_pair(poly, seg);
  std::swap(res.first, res.second);
  return res;
}

/// @ingroup geometry
/// @brief Computes the closest @ref tf::metric_point_pair between the objects.
template <std::size_t V0, typename Policy0, std::size_t V1, typename Policy1>
auto closest_metric_point_pair(const tf::polygon<V0, Policy0> &poly_in0,
                               const tf::polygon<V1, Policy1> &poly_in1) {
  const auto &poly0 = tf::inject_plane(poly_in0);
  const auto &poly1 = tf::inject_plane(poly_in1);

  std::size_t size = poly0.size();
  std::size_t prev = size - 1;
  auto best = closest_metric_point_pair(
      poly0, tf::make_segment_between_points(poly1[prev], poly1[0]));

  for (std::size_t i = 1; i < size; prev = i++) {
    auto tmp = closest_metric_point_pair(
        poly0, tf::make_segment_between_points(poly1[prev], poly1[i]));
    if (tmp.metric < best.metric)
      best = tmp;
    if (best.metric < std::numeric_limits<decltype(best.metric)>::epsilon())
      return best;
  }

  size = poly1.size();
  prev = size - 1;
  // we only need to check if the edges of poly1 intersect
  // poly0 (as all the edge combinations have been checked
  // by the closest_metric_point_pair calls until now)
  for (std::size_t i = 0; i < size; prev = i++) {
    auto ray = tf::make_ray_between_points(poly0[prev], poly0[i]);
    using RealT =
        tf::common_value<decltype(poly0[0][0]), decltype(poly1[0][0])>;
    auto hit_info =
        tf::ray_hit(ray, poly1, tf::make_ray_config(RealT(0), RealT(1)));
    if (hit_info) {
      return tf::make_metric_point_pair(RealT(0), hit_info.point,
                                        hit_info.point);
    }
  }

  return best;
}

} // namespace tf
