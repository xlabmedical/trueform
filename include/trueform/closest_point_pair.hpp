/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./vector.hpp"

namespace tf {

/// @brief Result of a nearest-point query between two primitives or trees.
///
/// Represents the closest pair of points—one from each object or tree—along with
/// the distance metric (typically squared distance). Used in dual-tree nearest neighbor queries
/// such as `tf::nearness_search(tree0, tree1)`.
/// Use `tf::make_closest_point_pair` to create an instance.
///
/// @tparam RealT The scalar coordinate type (e.g., float or double).
/// @tparam Dims The spatial dimension (e.g., 2 or 3).
template <typename RealT, std::size_t Dims> struct closest_point_pair {
  RealT metric;
  vector<RealT, Dims> first;
  vector<RealT, Dims> second;
};


/// @brief Construct a `closest_point_pair` from a distance and two spatial points.
///
/// Convenience function to create a `closest_point_pair<RealT, Dims>` instance without
/// explicitly specifying the type.
///
/// @tparam RealT The scalar coordinate type.
/// @tparam Dims The spatial dimension.
/// @param metric The distance metric (typically squared distance).
/// @param first The closest point from the first object or tree.
/// @param second The closest point from the second object or tree.
/// @return A `closest_point_pair<RealT, Dims>` instance.
template <typename RealT, std::size_t Dims>
auto make_closest_point_pair(RealT metric, vector<RealT, Dims> first,
                             vector<RealT, Dims> second) {
  return closest_point_pair<RealT, Dims>{metric, first, second};
}
} // namespace tf
