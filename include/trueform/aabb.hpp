/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./vector.hpp"

namespace tf {

/// @brief Axis-aligned bounding box in N-dimensional space.
///
/// Represents a rectangular region defined by its component-wise `min` and
/// `max` corners. Used extensively for spatial indexing, proximity queries, and
/// partitioning operations.
///
/// @tparam T The scalar type of the coordinates (e.g., float or double).
/// @tparam N The spatial dimension (e.g., 2 or 3).
template <typename T, std::size_t N> class aabb {
public:
  vector<T, N> min;
  vector<T, N> max;

  // Default constructor
  aabb() = default;

  // Constructor from min and max
  aabb(const vector<T, N> &min_, const vector<T, N> &max_)
      : min(min_), max(max_) {}

  /// @brief Compute the center point of the AABB.
  ///
  /// Returns the midpoint between `min` and `max`.
  ///
  /// @return A `vector<T, N>` representing the center.
  auto center() const -> vector<T, N> {
    return (min + max) * static_cast<T>(0.5);
  }

  /// @brief Compute the diagonal vector of the AABB.
  ///
  /// Returns the vector from `min` to `max`, representing the box’s size
  /// along each axis.
  ///
  /// @return A `vector<T, N>` representing the diagonal.
  auto diagonal() const -> vector<T, N> { return max - min; }

  /// @brief Return the dimensionality of the AABB.
  ///
  /// Provided as a compile-time constant.
  ///
  /// @return The number of spatial dimensions (N).
  constexpr auto size() -> std::size_t { return N; }
};

/// @brief Construct an AABB from `min` and `max` corners.
///
/// A convenience function equivalent to directly calling the `aabb<T, N>`
/// constructor.
///
/// @tparam T The scalar coordinate type.
/// @tparam N The spatial dimension.
/// @param min The lower corner of the bounding box.
/// @param max The upper corner of the bounding box.
/// @return An `aabb<T, N>` instance.
template <typename T, std::size_t N>
auto make_aabb(const vector<T, N> &min, const vector<T, N> &max) -> aabb<T, N> {
  return aabb<T, N>(min, max);
}

} // namespace tf
