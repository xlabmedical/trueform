/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./aabb.hpp"
#include "./dot.hpp"
#include "./plane.hpp"
#include "./sqrt.hpp"
#include "./value_type.hpp"
#include "./vector_like.hpp"

namespace tf {

/// @ingroup geometry
/// @brief Computes the squared Euclidean distance between two vectors.
/// @tparam N The dimensionality.
/// @tparam T0 The vector policy
/// @tparam T1 The vector policy
/// @param a First vector.
/// @param b Second vector.
/// @return Squared distance between a and b.
template <std::size_t N, typename T0, typename T1>
auto distance2(const vector_like<N, T0> &a, const vector_like<N, T1> &b)
    -> tf::common_value<T0, T1> {
  tf::common_value<T0, T1> sum{};
  for (std::size_t i = 0; i < N; ++i) {
    tf::common_value<T0, T1> d = a[i] - b[i];
    sum += d * d;
  }
  return sum;
}

/// @ingroup geometry
/// @brief Computes the Euclidean distance between two vectors.
/// @tparam N The dimensionality.
/// @tparam T0 The vector policy
/// @tparam T1 The vector policy
/// @param a First vector.
/// @param b Second vector.
/// @return Distance between a and b.
template <std::size_t N, typename T0, typename T1>
auto distance(const vector_like<N, T0> &a, const vector_like<N, T1> &b)
    -> tf::common_value<T0, T1> {
  return tf::sqrt(distance2(a, b));
}
/// @ingroup geometry
/// @brief Computes the squared distance between two AABBs.
/// The result is 0 if they overlap.
/// @tparam T The scalar type.
/// @tparam N The dimensionality.
/// @param a First AABB.
/// @param b Second AABB.
/// @return Squared distance between AABBs.
template <typename T, std::size_t N>
auto distance2(const aabb<T, N> &a, const aabb<T, N> &b) -> T {
  T dist2 = T{};
  for (std::size_t i = 0; i < N; ++i) {
    const auto d1 =
        std::max(a.min[i] - b.max[i], decltype(a.min[i] - b.max[i]){0});
    auto d2 = std::max(b.min[i] - a.max[i], decltype(a.min[i] - b.max[i]){0});
    d2 *= d1 == 0;
    dist2 += d1 * d1 + d2 * d2;
  }
  return dist2;
}

/// @ingroup geometry
/// @brief Computes the distance between two AABBs.
/// @tparam T The scalar type.
/// @tparam N The dimensionality.
/// @param a First AABB.
/// @param b Second AABB.
/// @return Distance between AABBs.
template <typename T, std::size_t N>
auto distance(const aabb<T, N> &a, const aabb<T, N> &b) -> T {
  return tf::sqrt(distance2(a, b));
}

/// @ingroup geometry
/// @brief Computes the squared distance from a point to an AABB.
/// @tparam N The dimensionality.
/// @tparam T The aabb value type
/// @tparam T1 The vector policy
/// @param _bbox The AABB.
/// @param _point The point.
/// @return Squared distance from point to AABB.
template <typename T, std::size_t N, typename T1>
auto distance2(const aabb<T, N> &_bbox, const vector_like<N, T1> &_point) {
  tf::common_value<T, T1> dist2{};
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

/// @ingroup geometry
/// @brief Computes the squared distance from a point to an AABB (reverse
/// argument order).
template <std::size_t N, typename T0, typename T1>
auto distance2(const vector_like<N, T0> &_point, const aabb<T1, N> &_bbox) {
  return distance2(_bbox, _point);
}

/// @ingroup geometry
/// @brief Computes the distance from a point to an AABB.
template <typename T, std::size_t N, typename T1>
auto distance(const aabb<T, N> &_bbox, const vector_like<N, T1> &_point) {
  return tf::sqrt(distance2(_bbox, _point));
}

/// @ingroup geometry
/// @brief Computes the distance from a point to an AABB (reverse argument
/// order).
template <std::size_t N, typename T0, typename T1>
auto distance(const vector_like<N, T0> &_point, const aabb<T1, N> &_bbox) {
  return tf::sqrt(distance2(_bbox, _point));
}

/// @ingroup geometry
/// @brief Computes the distance from a point to an AABB (reverse argument
/// order).
template <typename T, std::size_t Dims, typename T1>
auto distance(const plane<T, Dims> &p, const vector_like<Dims, T1> &pt) {
  return tf::dot(p.normal, pt) + p.d;
}

/// @ingroup geometry
/// @brief Computes the distance from a point to an AABB (reverse argument
/// order).
template <std::size_t Dims, typename T, typename T1>
auto distance(const vector_like<Dims, T> &pt, const plane<T1, Dims> &p) {
  return distance(p, pt);
}

/// @ingroup geometry
/// @brief Computes the distance from a point to an AABB (reverse argument
/// order).
template <typename T, std::size_t Dims, typename T1>
auto distance2(const plane<T, Dims> &p, const vector_like<Dims, T1> &pt) {
  auto d = distance(p, pt);
  return d * d;
}

/// @ingroup geometry
/// @brief Computes the distance from a point to an AABB (reverse argument
/// order).
template <std::size_t Dims, typename T, typename T1>
auto distance2(const vector_like<Dims, T> &pt, const plane<T1, Dims> &p) {
  return distance2(p, pt);
}
} // namespace tf
