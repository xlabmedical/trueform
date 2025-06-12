/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./point.hpp"
#include "./point_like.hpp"
#include "./value_type.hpp"

namespace tf {

/// @ingroup geometry
/// @brief A point and a metric
///
/// Represents a candidate closest point, including both the metric (e.g.,
/// squared distance) and the corresponding spatial position. Used during
/// nearest-neighbor traversal and refinement. Use `tf::make_metric_point` to
/// create an instance.
///
/// @tparam RealT The scalar coordinate type (e.g., float or double).
/// @tparam Dims The spatial dimension (e.g., 2 or 3).
template <typename RealT, std::size_t Dims> struct metric_point {
  RealT metric;
  tf::point<RealT, Dims> point;
};

/// @ingroup geometry
/// @brief Construct a `metric_point` object from a metric and a spatial
/// position.
///
/// Convenience function to create a `metric_point<RealT, Dims>` without
/// explicitly specifying the struct type.
///
/// @tparam RealT The scalar coordinate type.
/// @tparam Dims The spatial dimension.
/// @tparam T The point policy
/// @param metric The distance metric (typically squared distance).
/// @param point The closest spatial point corresponding to the metric.
/// @return A `closest_point<RealT, Dims>` instance.
template <typename RealT, std::size_t Dims, typename T>
auto make_metric_point(RealT metric, point_like<Dims, T> point) {
  return metric_point<tf::common_value<RealT, T>, Dims>{metric, point};
}

template <typename RealT, std::size_t Dims>
auto min(const metric_point<RealT, Dims> &lhs,
         const metric_point<RealT, Dims> &rhs)
    -> const metric_point<RealT, Dims> & {
  if (lhs.metric < rhs.metric)
    return lhs;
  return rhs;
}

template <typename RealT, std::size_t Dims>
auto max(const metric_point<RealT, Dims> &lhs,
         const metric_point<RealT, Dims> &rhs)
    -> const metric_point<RealT, Dims> & {
  if (lhs.metric > rhs.metric)
    return lhs;
  return rhs;
}
} // namespace tf
