/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./aabb.hpp"
#include "./vector.hpp"
#include "./vector_view.hpp"
#include <cmath>

namespace tf {

/// @brief Computes the squared Euclidean distance between two vectors.
/// @tparam T The scalar type.
/// @tparam N The dimensionality.
/// @param a First vector.
/// @param b Second vector.
/// @return Squared distance between a and b.
template <typename T, std::size_t N>
auto distance2(const vector<T, N> &a, const vector<T, N> &b) -> T {
  T sum = T{};
  for (std::size_t i = 0; i < N; ++i) {
    T d = a[i] - b[i];
    sum += d * d;
  }
  return sum;
}

/// @brief Computes the Euclidean distance between two vectors.
/// @tparam T The scalar type.
/// @tparam N The dimensionality.
/// @param a First vector.
/// @param b Second vector.
/// @return Distance between a and b.
template <typename T, std::size_t N>
auto distance(const vector<T, N> &a, const vector<T, N> &b) -> T {
  return std::sqrt(distance2(a, b));
}

/// @brief Computes the squared Euclidean distance between two vector views.
/// @tparam T The scalar type.
/// @tparam N The dimensionality.
/// @param a First vector view.
/// @param b Second vector view.
/// @return Squared distance between a and b.
template <typename T, std::size_t N>
auto distance2(const vector_view<T, N> &a, const vector_view<T, N> &b) -> T {
  T sum = T{};
  for (std::size_t i = 0; i < N; ++i) {
    T d = a[i] - b[i];
    sum += d * d;
  }
  return sum;
}

/// @brief Computes the Euclidean distance between two vector views.
/// @tparam T The scalar type.
/// @tparam N The dimensionality.
/// @param a First vector view.
/// @param b Second vector view.
/// @return Distance between a and b.
template <typename T, std::size_t N>
auto distance(const vector_view<T, N> &a, const vector_view<T, N> &b) -> T {
  return std::sqrt(distance2(a, b));
}

/// @brief Computes the squared distance between two AABBs.
/// The result is 0 if they overlap.
/// @tparam T The scalar type.
/// @tparam N The dimensionality.
/// @param a First AABB.
/// @param b Second AABB.
/// @return Squared distance between AABBs.
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

/// @brief Computes the distance between two AABBs.
/// @tparam T The scalar type.
/// @tparam N The dimensionality.
/// @param a First AABB.
/// @param b Second AABB.
/// @return Distance between AABBs.
template <typename T, std::size_t N>
auto distance(const aabb<T, N> &a, const aabb<T, N> &b) -> T {
  return std::sqrt(distance2(a, b));
}

/// @brief Computes the squared distance from a point to an AABB.
/// @tparam T The scalar type.
/// @tparam N The dimensionality.
/// @param _bbox The AABB.
/// @param _point The point.
/// @return Squared distance from point to AABB.
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

/// @brief Computes the squared distance from a point to an AABB (reverse argument order).
template <typename T, std::size_t N>
auto distance2(const vector<T, N> &_point, const aabb<T, N> &_bbox) {
  return distance2(_bbox, _point);
}

/// @brief Computes the distance from a point to an AABB.
template <typename T, std::size_t N>
auto distance(const aabb<T, N> &_bbox, const vector<T, N> &_point) {
  return std::sqrt(distance2(_bbox, _point));
}

/// @brief Computes the distance from a point to an AABB (reverse argument order).
template <typename T, std::size_t N>
auto distance(const vector<T, N> &_point, const aabb<T, N> &_bbox) {
  return std::sqrt(distance2(_bbox, _point));
}

/// @brief Computes the squared distance from a vector view to an AABB.
template <typename T, std::size_t N>
auto distance2(const aabb<T, N> &_bbox, const vector_view<T, N> &_point) {
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

/// @brief Computes the squared distance from a vector view to an AABB (reverse argument order).
template <typename T, std::size_t N>
auto distance2(const vector_view<T, N> &_point, const aabb<T, N> &_bbox) {
  return distance2(_bbox, _point);
}

/// @brief Computes the distance from a vector view to an AABB.
template <typename T, std::size_t N>
auto distance(const aabb<T, N> &_bbox, const vector_view<T, N> &_point) {
  return std::sqrt(distance2(_bbox, _point));
}

/// @brief Computes the distance from a vector view to an AABB (reverse argument order).
template <typename T, std::size_t N>
auto distance(const vector_view<T, N> &_point, const aabb<T, N> &_bbox) {
  return std::sqrt(distance2(_bbox, _point));
}
} // namespace tf
