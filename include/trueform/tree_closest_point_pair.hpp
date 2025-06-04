/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./closest_point_pair.hpp"
#include <utility>

namespace tf {

/// @brief Result of a nearest-point query between two spatial trees.
///
/// Represents the closest pair of primitives found between two trees, along with
/// their closest points and the associated distance metric. This structure is returned
/// by `tf::nearness_search(tree0, tree1)`.
///
/// Includes utility accessors for checking validity and retrieving the distance metric.
///
/// @tparam Index The type used for primitive identifiers (typically an integer).
/// @tparam RealT The scalar coordinate type (e.g., float or double).
/// @tparam Dims The spatial dimension (typically 2 or 3).
template <typename Index, typename RealT, std::size_t Dims>
struct tree_closest_point_pair {
  using index_t = Index;
  using real_t = RealT;
  using element_t = std::pair<Index, Index>;
  using closest_point_t = tf::closest_point_pair<RealT, Dims>;
  //
  static constexpr Index no_id = -1;
  std::pair<Index, Index> elements{no_id, no_id};
  closest_point_pair<RealT, Dims> points;

  operator bool() const { return elements.first != no_id; }
  auto metric() const -> real_t { return points.metric; }
};
} // namespace tf
