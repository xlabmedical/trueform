/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./point.hpp"
#include "./value_type.hpp"
#include "./vector.hpp"

namespace tf {

/// @ingroup geometry
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
  /// @brief The minimal corner
  point<T, N> min;
  /// @brief The maximal corner
  point<T, N> max;

  // Default constructor
  aabb() = default;

  // Constructor from min and max
  aabb(const point<T, N> &min_, const point<T, N> &max_)
      : min(min_), max(max_) {}

  /// @brief Compute the center point of the AABB.
  ///
  /// Returns the midpoint between `min` and `max`.
  ///
  /// @return A `vector<T, N>` representing the center.
  auto center() const -> point<T, N> {
    return tf::make_point((min.as_vector_view() + max.as_vector_view()) *
                          static_cast<T>(0.5));
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

/// @ingroup geometry
/// @brief Construct an AABB from `min` and `max` corners.
///
/// A convenience function equivalent to directly calling the `aabb<T, N>`
/// constructor.
///
/// @tparam N The spatial dimension.
/// @tparam T0 The vector policy
/// @tparam T1 The vector policy
/// @param min The lower corner of the bounding box.
/// @param max The upper corner of the bounding box.
/// @return An `aabb<T, N>` instance.
template <std::size_t N, typename T0, typename T1>
auto make_aabb(const point_like<N, T0> &min, const point_like<N, T1> &max)
    -> aabb<tf::common_value<T0, T1>, N> {
  return aabb<tf::common_value<T0, T1>, N>(min, max);
}

} // namespace tf
