/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./value_type.hpp"
#include "./vector.hpp"

namespace tf {

/// @ingroup geometry
/// @brief Represents a line in D-dimensional space.
///
/// A line is defined by a point of origin and a direction vector. The direction
/// vector does not need to be normalized and the line is considered to extend
/// infinitely in both directions.
///
/// @tparam T The scalar type (e.g., float, double).
/// @tparam Dims The number of spatial dimensions.
template <typename T, std::size_t Dims> struct line {
  tf::vector<T, Dims> origin;    ///< A point on the line.
  tf::vector<T, Dims> direction; ///< The direction vector of the line.
};

/// @ingroup geometry
/// @brief Constructs a line from an origin and a direction vector.
///
/// The direction vector defines the orientation of the line, which extends
/// infinitely in both directions. The resulting line's value type is deduced
/// from the common type of the origin and direction components.
///
/// @tparam T0 The type of the origin vector's components.
/// @tparam T1 The type of the direction vector's components.
/// @tparam Dims The spatial dimension of the vectors.
/// @param origin A point on the line.
/// @param direction The direction of the line (not necessarily normalized).
/// @return A line object.
template <typename T0, std::size_t Dims, typename T1>
auto make_line(const tf::vector_like<Dims, T0> &origin,
               const tf::vector_like<Dims, T1> &direction) {
  return line<tf::common_value<T0, T1>, Dims>{origin, direction};
}

/// @ingroup geometry
/// @brief Constructs a line from two points.
///
/// The direction of the line is computed as `end - origin`. The line passes
/// through both `origin` and `end`, and extends infinitely in both directions.
///
/// @tparam T0 The type of the origin point.
/// @tparam T1 The type of the end point.
/// @tparam Dims The spatial dimension of the points.
/// @param origin The first point on the line.
/// @param end The second point on the line (used to compute the direction).
/// @return A line object.
template <typename T0, std::size_t Dims, typename T1>
auto make_line_between_points(const tf::vector_like<Dims, T0> &origin,
                              const tf::vector_like<Dims, T1> &end) {
  return line<tf::common_value<T0, T1>, Dims>{origin, end - origin};
}

} // namespace tf
