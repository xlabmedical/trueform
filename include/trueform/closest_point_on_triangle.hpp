/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./dot.hpp"
#include "./vector.hpp"
#include "./vector_view.hpp"

namespace tf {
template <typename Range, typename T>
auto closest_point_on_triangle(const Range &triangle,
                               const tf::vector_view<T, 3> &point) {
  auto ab = triangle[1] - triangle[0];
  auto ac = triangle[2] - triangle[0];
  auto ap = point - triangle[0];
  auto d1 = tf::dot(ab, ap);
  auto d2 = tf::dot(ac, ap);
  if (d1 <= 0 && d2 <= 0) {
    // barycentric coordinates (1,0,0)
    return triangle[0];
  }
  auto bp = point - triangle[1];
  auto d3 = tf::dot(ab, bp);
  auto d4 = tf::dot(ac, bp);
  if (d3 >= 0 && d4 <= d3) {
    // barycentric coordinates (0,1,0)
    return triangle[1];
  }
  auto vc = d1 * d4 - d3 * d2;
  if (vc <= 0 && d1 >= 0 && d3 <= 0) {
    // barycentric coordinates (1-v,v,0)
    auto v = d1 / (d1 - d3);
    return triangle[0] + v * ab;
  }
  auto cp = point - triangle[2];
  auto d5 = tf::dot(ab, cp);
  auto d6 = tf::dot(ac, cp);
  if (d6 >= 0 && d5 <= d6) {
    // barycentric coordinates (0,0,1)
    return triangle[2];
  }
  auto vb = d5 * d2 - d1 * d6;
  if (vb <= 0 && d2 >= 0 && d6 <= 0) {
    // barycentric coordinates (1-w,0,w)
    auto w = d2 / (d2 - d6);
    return triangle[0] + w * ac;
  }
  auto va = d3 * d6 - d5 * d4;
  if (va <= 0 && (d4 - d3) >= 0 && (d5 - d6) >= 0) {
    // barycentric coordinates (0,1-w,w)
    auto w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
    return triangle[1] + w * (triangle[2] - triangle[1]);
  }
  // barycentric coordinates (u,v,w)
  auto denom = 1 / (va + vb + vc);
  auto v = vb * denom;
  auto w = vc * denom;
  return triangle[0] + ab * v + ac * w;
}

template <typename Range, typename T>
auto closest_point_on_triangle(const Range &triangle,
                               const tf::vector<T, 3> &point) {
  return closest_point_on_triangle(triangle,
                                   tf::make_vector_view<3>(point.begin()));
}
} // namespace tf
