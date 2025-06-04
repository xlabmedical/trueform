/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./vector.hpp"

namespace tf {

/// @brief Result of a nearest-point query against a spatial primitive.
///
/// Represents a candidate closest point, including both the metric (e.g., squared distance)
/// and the corresponding spatial position. Used during nearest-neighbor traversal and refinement.
/// Use `tf::make_closest_point` to create an instance.
///
/// @tparam RealT The scalar coordinate type (e.g., float or double).
/// @tparam Dims The spatial dimension (e.g., 2 or 3).
template <typename RealT, std::size_t Dims> struct closest_point {
  RealT metric;
  vector<RealT, Dims> point;
};


/// @brief Construct a `closest_point` object from a metric and a spatial position.
///
/// Convenience function to create a `closest_point<RealT, Dims>` without explicitly
/// specifying the struct type.
///
/// @tparam RealT The scalar coordinate type.
/// @tparam Dims The spatial dimension.
/// @param metric The distance metric (typically squared distance).
/// @param point The closest spatial point corresponding to the metric.
/// @return A `closest_point<RealT, Dims>` instance.
template <typename RealT, std::size_t Dims>
auto make_closest_point(RealT metric, vector<RealT, Dims> point) {
  return closest_point<RealT, Dims>{metric, point};
}
} // namespace tf
