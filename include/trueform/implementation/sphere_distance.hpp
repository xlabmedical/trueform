/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "../closest_point_parametric.hpp"
#include "../point_like.hpp"
#include "../sphere.hpp"
#include <limits>

namespace tf::implementation {
template <typename T0, std::size_t Dims, typename T1>
auto sphere_distance(const sphere<T0, Dims> &s,
                     const point_like<Dims, T1> &pt) {
  auto d2 = (s.origin - pt).length2();
  if (d2 + std::numeric_limits<decltype(d2)>::epsilon() < s.r * s.r)
    return decltype(d2)(0);
  return tf::sqrt(d2) - s.r;
}

template <typename T0, std::size_t Dims, typename T1>
auto sphere_distance2(const sphere<T0, Dims> &s,
                      const point_like<Dims, T1> &pt) {
  auto d = sphere_distance(s, pt);
  return d * d;
}

template <typename T0, std::size_t Dims, typename T1>
auto sphere_distance(const sphere<T0, Dims> &s, const ray<T1, Dims> &r) {
  auto t = closest_point_parametric(r, s.origin);
  return sphere_distance(s, r.origin + t * r.direction);
}

template <typename T0, std::size_t Dims, typename T1>
auto sphere_distance2(const sphere<T0, Dims> &s, const ray<T1, Dims> &r) {

  auto d = sphere_distance(s, r);
  return d * d;
}

template <typename T0, std::size_t Dims, typename T1>
auto sphere_distance(const sphere<T0, Dims> &s, const line<T1, Dims> &l) {
  auto t = closest_point_parametric(l, s.origin);
  return sphere_distance(s, l.origin + t * l.direction);
}

template <typename T0, std::size_t Dims, typename T1>
auto sphere_distance2(const sphere<T0, Dims> &s, const line<T1, Dims> &l) {

  auto d = sphere_distance(s, l);
  return d * d;
}

template <typename T0, std::size_t Dims, typename T1>
auto sphere_distance(const sphere<T0, Dims> &s, const segment<T1> &seg) {
  auto t = closest_point_parametric(seg, s.origin);
  return sphere_distance(s, seg[0] + t * (seg[1] - seg[0]));
}

template <typename T0, std::size_t Dims, typename T1>
auto sphere_distance2(const sphere<T0, Dims> &s, const segment<T1> &seg) {
  auto d = sphere_distance(s, seg);
  return d * d;
}
} // namespace tf::implementation
