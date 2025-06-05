/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./aabb.hpp"
#include "./vector.hpp"
#include "./vector_view.hpp"

namespace tf {

/// @ingroup geometry
/// @brief Check whether two geometric primitives intersect.
///
/// This overload of `intersects` checks for intersection between
/// two AABBs.
///
/// @return `true` if the primitives intersect; otherwise `false`.
template <typename T, std::size_t N>
auto intersects(const aabb<T, N> &a, const aabb<T, N> &b) -> bool {
  for (std::size_t i = 0; i < N; ++i) {
    if (a.max[i] < b.min[i] || b.max[i] < a.min[i])
      return false;
  }
  return true;
}

/// @ingroup geometry
/// @brief Check whether two geometric primitives intersect within epsilon.
///
/// This overload of `intersects` checks for intersection between specific types.
///
/// @return `true` if the primitives intersect; otherwise `false`.

template <typename T, std::size_t N>
auto intersects(const aabb<T, N> &a, const aabb<T, N> &b, T epsilon) -> bool {
  for (std::size_t i = 0; i < N; ++i) {
    if (a.max[i] + epsilon < b.min[i] || b.max[i] + epsilon < a.min[i])
      return false;
  }
  return true;
}

/// @ingroup geometry
/// @brief Check whether two geometric primitives intersect.
///
/// This overload of `intersects` checks for intersection between specific types.
///
/// @return `true` if the primitives intersect; otherwise `false`.

template <typename T, std::size_t N>
auto intersects(const vector<T, N> &point, const aabb<T, N> &box) -> bool {
  for (std::size_t i = 0; i < N; ++i) {
    if (point[i] < box.min[i] || point[i] > box.max[i])
      return false;
  }
  return true;
}

/// @ingroup geometry
/// @brief Check whether two geometric primitives intersect within epsilon.
///
/// This overload of `intersects` checks for intersection between specific types.
///
/// @return `true` if the primitives intersect; otherwise `false`.

template <typename T, std::size_t N>
auto intersects(const aabb<T, N> &box, const vector<T, N> &point) -> bool {
  return intersects(point, box);
}

/// @ingroup geometry
/// @brief Check whether two geometric primitives intersect within epsilon.
///
/// This overload of `intersects` checks for intersection between specific types.
///
/// @return `true` if the primitives intersect; otherwise `false`.

template <typename T, std::size_t N>
auto intersects(const vector<T, N> &point, const aabb<T, N> &box, T epsilon)
    -> bool {
  for (std::size_t i = 0; i < N; ++i) {
    if (point[i] + epsilon < box.min[i] || point[i] - epsilon > box.max[i])
      return false;
  }
  return true;
}

/// @ingroup geometry
/// @brief Check whether two geometric primitives intersect within epsilon.
///
/// This overload of `intersects` checks for intersection between specific types.
///
/// @return `true` if the primitives intersect; otherwise `false`.
template <typename T, std::size_t N>
auto intersects(const aabb<T, N> &box, const vector<T, N> &point, T epsilon)
    -> bool {
  return intersects(point, box, epsilon);
}

/// @ingroup geometry
/// @brief Check whether two geometric primitives intersect.
///
/// This overload of `intersects` checks for intersection between specific types.
///
/// @return `true` if the primitives intersect; otherwise `false`.
template <typename T, std::size_t N>
auto intersects(const vector_view<T, N> &point, const aabb<T, N> &box) -> bool {
  for (std::size_t i = 0; i < N; ++i) {
    if (point[i] < box.min[i] || point[i] > box.max[i])
      return false;
  }
  return true;
}

/// @ingroup geometry
/// @brief Check whether two geometric primitives intersect.
///
/// This overload of `intersects` checks for intersection between specific types.
/// Requires appropriate intersection logic for the given argument types.
///
/// @return `true` if the primitives intersect; otherwise `false`.

template <typename T, std::size_t N>
auto intersects(const aabb<T, N> &box, const vector_view<T, N> &point) -> bool {
  return intersects(point, box);
}

/// @ingroup geometry
/// @brief Check whether two geometric primitives intersect within epsilon.
///
/// This overload of `intersects` checks for intersection between specific types.
///
/// @return `true` if the primitives intersect; otherwise `false`.
template <typename T, std::size_t N>
auto intersects(const vector_view<T, N> &point, const aabb<T, N> &box,
                T epsilon) -> bool {
  for (std::size_t i = 0; i < N; ++i) {
    if (point[i] + epsilon < box.min[i] || point[i] - epsilon > box.max[i])
      return false;
  }
  return true;
}

/// @ingroup geometry
/// @brief Check whether two geometric primitives intersect within epsilon.
///
/// This overload of `intersects` checks for intersection between specific types.
///
/// @return `true` if the primitives intersect; otherwise `false`.
template <typename T, std::size_t N>
auto intersects(const aabb<T, N> &box, const vector_view<T, N> &point,
                T epsilon) -> bool {
  return intersects(point, box, epsilon);
}

} // namespace tf

