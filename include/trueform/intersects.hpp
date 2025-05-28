/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./aabb.hpp"
#include "./vector.hpp"

namespace tf {

template <typename T, std::size_t N>
auto intersects(const aabb<T, N> &a, const aabb<T, N> &b) -> bool {
  for (std::size_t i = 0; i < N; ++i) {
    if (a.max[i] < b.min[i] || b.max[i] < a.min[i])
      return false;
  }
  return true;
}

template <typename T, std::size_t N>
auto intersects(const aabb<T, N> &a, const aabb<T, N> &b, T epsilon) -> bool {
  for (std::size_t i = 0; i < N; ++i) {
    if (a.max[i] + epsilon < b.min[i] || b.max[i] + epsilon < a.min[i])
      return false;
  }
  return true;
}

template <typename T, std::size_t N>
auto intersects(const vector<T, N> &point, const aabb<T, N> &box) -> bool {
  for (std::size_t i = 0; i < N; ++i) {
    if (point[i] < box.min[i] || point[i] > box.max[i])
      return false;
  }
  return true;
}

template <typename T, std::size_t N>
auto intersects(const aabb<T, N> &box, const vector<T, N> &point) -> bool {
  return intersects(point, box);
}

template <typename T, std::size_t N>
auto intersects(const vector<T, N> &point, const aabb<T, N> &box, T epsilon)
    -> bool {
  for (std::size_t i = 0; i < N; ++i) {
    if (point[i] + epsilon < box.min[i] || point[i] - epsilon > box.max[i])
      return false;
  }
  return true;
}

template <typename T, std::size_t N>
auto intersects(const aabb<T, N> &box, const vector<T, N> &point, T epsilon)
    -> bool {
  return intersects(point, box, epsilon);
}

} // namespace tf
