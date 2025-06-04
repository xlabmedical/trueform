/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./closest_point.hpp"

namespace tf {

/// @brief Result of a nearest-point query against a spatial tree.
///
/// Represents the closest primitive and corresponding point found during a spatial query.
/// The result includes the primitive's index and its nearest point, along with the distance metric.
///
/// This struct is returned by `tf::nearness_search(...)` when querying a single tree,
/// and provides utilities to access the result’s validity and metric.
///
/// @tparam Index The type used for primitive identifiers (typically an integer).
/// @tparam RealT The scalar coordinate type (e.g., float or double).
/// @tparam Dims The spatial dimension (typically 2 or 3).
template <typename Index, typename RealT, std::size_t Dims>
struct tree_closest_point {
  using index_t = Index;
  using real_t = RealT;
  using element_t = Index;
  using closest_point_t = tf::closest_point<RealT, Dims>;
  //
  static constexpr Index no_id = -1;
  Index element{no_id};
  tf::closest_point<RealT, Dims> point;

  operator bool() const { return element != no_id; }
  auto metric() const -> real_t { return point.metric; }
};
} // namespace tf
