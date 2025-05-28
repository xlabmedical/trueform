/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./aabb.hpp"
#include "./vector.hpp"
#include <cmath>

namespace tf {

// squared distance between two vectors
template <typename T, std::size_t N>
auto distance2(const vector<T, N> &a, const vector<T, N> &b) -> T {
  T sum = T{};
  for (std::size_t i = 0; i < N; ++i) {
    T d = a[i] - b[i];
    sum += d * d;
  }
  return sum;
}

// Euclidean distance
template <typename T, std::size_t N>
auto distance(const vector<T, N> &a, const vector<T, N> &b) -> T {
  return std::sqrt(distance2(a, b));
}

// squared distance between AABBs (0 if they overlap)
template <typename T, std::size_t N>
auto distance2(const aabb<T, N> &a, const aabb<T, N> &b) -> T {
  T sum = T{};
  for (std::size_t i = 0; i < N; ++i) {
    if (a.max[i] < b.min[i]) {
      T tmp = b.min[i] - a.max[i];
      sum += tmp * tmp;
    } else if (b.max[i] < a.min[i]) {
      T tmp = a.min[i] - b.max[i];
      sum += tmp * tmp;
    }
  }
  return sum;
}

// distance between AABBs
template <typename T, std::size_t N>
auto distance(const aabb<T, N> &a, const aabb<T, N> &b) -> T {
  return std::sqrt(distance2(a, b));
}

template <typename T, std::size_t N>
auto distance2(const aabb<T, N> &_bbox, const vector<T, N> &_point) {
  decltype(_bbox.min[0] - _bbox.min[0]) dist2{};
  const auto &min = _bbox.min;
  const auto &max = _bbox.max;
  for (int i = 0; i < int(N); ++i) {
    auto outside_low =
        std::max(min[i] - _point[i], decltype(min[i] - _point[i]){0});
    auto outside_high =
        std::max(_point[i] - max[i], decltype(_point[i] - max[i]){0});
    outside_high *= outside_low == 0;
    dist2 += outside_low * outside_low + outside_high * outside_high;
  }
  return dist2;
}

template <typename T, std::size_t N>
auto distance2(const vector<T, N> &_point, const aabb<T, N> &_bbox) {
  return distance2(_bbox, _point);
}

template <typename T, std::size_t N>
auto distance(const aabb<T, N> &_bbox, const vector<T, N> &_point) {
  return std::sqrt(distance2(_bbox, _point));
}

template <typename T, std::size_t N>
auto distance(const vector<T, N> &_point, const aabb<T, N> &_bbox) {
  return std::sqrt(distance2(_bbox, _point));
}

} // namespace tf
