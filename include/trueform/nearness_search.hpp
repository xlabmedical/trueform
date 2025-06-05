/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./implementation/tree_closest_point.hpp"
#include "./implementation/tree_closest_point_pair.hpp"
#include "./implementation/tree_closest_point_using_sort_by_level.hpp"
#include "./implementation/tree_tree_proximity.hpp"
#include "./mod_tree.hpp"
#include "./tree.hpp"
#include "./tree_knn.hpp"

namespace tf {

namespace strategy {
struct top_k_sorted_t {};
static constexpr top_k_sorted_t top_k_sorted;
struct priority_queue_t {};
static constexpr priority_queue_t priority_queue;
} // namespace strategy

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f) {
  tf::implementation::tree_closest_point<Index, RealT, N> result{
      std::numeric_limits<RealT>::max()};
  tf::implementation::tree_closest_point_using_sort_by_level(
      tree.nodes(), tree.ids(), aabb_metric, closest_point_f, result);
  return result.point;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::mod_tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f) {
  tf::implementation::tree_closest_point<Index, RealT, N> result{
      std::numeric_limits<RealT>::max()};
  tf::implementation::tree_closest_point_using_sort_by_level(
      tree.main_tree().nodes(), tree.main_tree().ids(), aabb_metric,
      closest_point_f, result);
  tf::implementation::tree_closest_point_using_sort_by_level(
      tree.delta_tree().nodes(), tree.delta_tree().ids(), aabb_metric,
      closest_point_f, result);
  return result.point;
}

/// @ingroup spatial_queries
/// @brief Perform a nearest-point spatial query against a single tree structure.
///
/// Traverses the tree to find the closest primitive to an implicit query point,
/// based on user-provided AABB distance and primitive distance callbacks.
/// The tree is searched using a top-k sorted traversal strategy.
///
/// @param tree The spatial tree to query.
/// @param aabb_metric A function that estimates the distance to a node's AABB.
///                    Signature: `(const tf::aabb<RealT, N>& aabb) -> RealT`
/// @param closest_point_f A function that evaluates the true distance to a primitive.
///                        Signature: `(Index id) -> tf::closest_point<RealT, N>`
///
/// @return tf::tree_closest_point<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f) {
  return nearness_search(strategy::top_k_sorted, tree, aabb_metric,
                         closest_point_f);
}

/// @ingroup spatial_queries
/// @brief Perform a nearest-point spatial query against a single tree structure.
///
/// Traverses the tree to find the closest primitive to an implicit query point,
/// based on user-provided AABB distance and primitive distance callbacks.
/// The tree is searched using a top-k sorted traversal strategy.
///
/// @param tree The spatial tree to query.
/// @param aabb_metric A function that estimates the distance to a node's AABB.
///                    Signature: `(const tf::aabb<RealT, N>& aabb) -> RealT`
/// @param closest_point_f A function that evaluates the true distance to a primitive.
///                        Signature: `(Index id) -> tf::closest_point<RealT, N>`
///
/// @return tf::tree_closest_point<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::mod_tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f) {
  return nearness_search(strategy::top_k_sorted, tree, aabb_metric,
                         closest_point_f);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::priority_queue_t,
                     const tf::tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f) {
  tf::implementation::tree_closest_point<Index, RealT, N> result{
      std::numeric_limits<RealT>::max()};
  tf::implementation::tree_closest_point_using_heap(
      tree.nodes(), tree.ids(), aabb_metric, closest_point_f, result);
  return result.point;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::priority_queue_t,
                     const tf::mod_tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f) {
  tf::implementation::tree_closest_point<Index, RealT, N> result{
      std::numeric_limits<RealT>::max()};
  tf::implementation::tree_closest_point_using_heap(
      tree.main_tree().nodes(), tree.main_tree().ids(), aabb_metric,
      closest_point_f, result);
  tf::implementation::tree_closest_point_using_heap(
      tree.delta_tree().nodes(), tree.delta_tree().ids(), aabb_metric,
      closest_point_f, result);
  return result.point;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{
      std::numeric_limits<RealT>::max()};
  tf::implementation::tree_tree_proximity_sort(tree0, tree1, aabb_metrics_f,
                                               closest_points_f, result);
  return result.points;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{
      std::numeric_limits<RealT>::max()};

  tf::implementation::tree_tree_proximity_sort(
      tree0.main_tree(), tree1.main_tree(), aabb_metrics_f, closest_points_f,
      result);
  tf::implementation::tree_tree_proximity_sort(
      tree0.main_tree(), tree1.delta_tree(), aabb_metrics_f, closest_points_f,
      result);
  tf::implementation::tree_tree_proximity_sort(
      tree0.delta_tree(), tree1.main_tree(), aabb_metrics_f, closest_points_f,
      result);
  tf::implementation::tree_tree_proximity_sort(
      tree0.delta_tree(), tree1.delta_tree(), aabb_metrics_f, closest_points_f,
      result);
  return result.points;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{
      std::numeric_limits<RealT>::max()};

  tf::implementation::tree_tree_proximity_sort(
      tree0.main_tree(), tree1, aabb_metrics_f, closest_points_f, result);
  tf::implementation::tree_tree_proximity_sort(
      tree0.delta_tree(), tree1, aabb_metrics_f, closest_points_f, result);

  return result.points;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{
      std::numeric_limits<RealT>::max()};

  tf::implementation::tree_tree_proximity_sort(
      tree0, tree1.main_tree(), aabb_metrics_f, closest_points_f, result);
  tf::implementation::tree_tree_proximity_sort(
      tree0, tree1.delta_tree(), aabb_metrics_f, closest_points_f, result);

  return result.points;
}


/// @ingroup spatial_queries
/// @brief Perform a nearest-point spatial query between two tree structures.
///
/// Traverses both trees in tandem to find the closest pair of primitives between them,
/// using user-provided AABB distance and primitive distance callbacks. 
///
/// @param tree0 The first spatial tree to query.
/// @param tree1 The second spatial tree to query.
/// @param aabb_metrics_f A function that estimates the distances between two AABBs.
///                       Signature: `(const tf::aabb<RealT, N>& a, const tf::aabb<RealT, N>& b) -> tf::aabb_metrics<RealT>`
/// @param closest_points_f A function that evaluates the true distance between a pair of primitives.
///                         Signature: `(Index id0, Index id1) -> tf::closest_point_pair<RealT, N>`
///
/// @return tf::tree_closest_point_pair<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f);
}

/// @ingroup spatial_queries
/// @brief Perform a nearest-point spatial query between two tree structures.
///
/// Traverses both trees in tandem to find the closest pair of primitives between them,
/// using user-provided AABB distance and primitive distance callbacks. 
///
/// @param tree0 The first spatial tree to query.
/// @param tree1 The second spatial tree to query.
/// @param aabb_metrics_f A function that estimates the distances between two AABBs.
///                       Signature: `(const tf::aabb<RealT, N>& a, const tf::aabb<RealT, N>& b) -> tf::aabb_metrics<RealT>`
/// @param closest_points_f A function that evaluates the true distance between a pair of primitives.
///                         Signature: `(Index id0, Index id1) -> tf::closest_point_pair<RealT, N>`
///
/// @return tf::tree_closest_point_pair<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f);
}

/// @ingroup spatial_queries
/// @brief Perform a nearest-point spatial query between two tree structures.
///
/// Traverses both trees in tandem to find the closest pair of primitives between them,
/// using user-provided AABB distance and primitive distance callbacks. 
///
/// @param tree0 The first spatial tree to query.
/// @param tree1 The second spatial tree to query.
/// @param aabb_metrics_f A function that estimates the distances between two AABBs.
///                       Signature: `(const tf::aabb<RealT, N>& a, const tf::aabb<RealT, N>& b) -> tf::aabb_metrics<RealT>`
/// @param closest_points_f A function that evaluates the true distance between a pair of primitives.
///                         Signature: `(Index id0, Index id1) -> tf::closest_point_pair<RealT, N>`
///
/// @return tf::tree_closest_point_pair<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f);
}

/// @ingroup spatial_queries
/// @brief Perform a nearest-point spatial query between two tree structures.
///
/// Traverses both trees in tandem to find the closest pair of primitives between them,
/// using user-provided AABB distance and primitive distance callbacks. 
///
/// @param tree0 The first spatial tree to query.
/// @param tree1 The second spatial tree to query.
/// @param aabb_metrics_f A function that estimates the distances between two AABBs.
///                       Signature: `(const tf::aabb<RealT, N>& a, const tf::aabb<RealT, N>& b) -> tf::aabb_metrics<RealT>`
/// @param closest_points_f A function that evaluates the true distance between a pair of primitives.
///                         Signature: `(Index id0, Index id1) -> tf::closest_point_pair<RealT, N>`
///
/// @return tf::tree_closest_point_pair<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::priority_queue_t,
                     const tf::tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{
      std::numeric_limits<RealT>::max()};
  tf::implementation::tree_tree_proximity_heap(tree0, tree1, aabb_metrics_f,
                                               closest_points_f, result);
  return result.points;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::priority_queue_t,
                     const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{
      std::numeric_limits<RealT>::max()};

  tf::implementation::tree_tree_proximity_heap(
      tree0.main_tree(), tree1.main_tree(), aabb_metrics_f, closest_points_f,
      result);
  tf::implementation::tree_tree_proximity_heap(
      tree0.main_tree(), tree1.delta_tree(), aabb_metrics_f, closest_points_f,
      result);
  tf::implementation::tree_tree_proximity_heap(
      tree0.delta_tree(), tree1.main_tree(), aabb_metrics_f, closest_points_f,
      result);
  tf::implementation::tree_tree_proximity_heap(
      tree0.delta_tree(), tree1.delta_tree(), aabb_metrics_f, closest_points_f,
      result);
  return result.points;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::priority_queue_t,
                     const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{
      std::numeric_limits<RealT>::max()};

  tf::implementation::tree_tree_proximity_heap(
      tree0.main_tree(), tree1, aabb_metrics_f, closest_points_f, result);
  tf::implementation::tree_tree_proximity_heap(
      tree0.delta_tree(), tree1, aabb_metrics_f, closest_points_f, result);

  return result.points;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::priority_queue_t,
                     const tf::tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{
      std::numeric_limits<RealT>::max()};

  tf::implementation::tree_tree_proximity_heap(
      tree0, tree1.main_tree(), aabb_metrics_f, closest_points_f, result);
  tf::implementation::tree_tree_proximity_heap(
      tree0, tree1.delta_tree(), aabb_metrics_f, closest_points_f, result);

  return result.points;
}

// nearness in search_radius

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     RealT radius) {
  tf::implementation::tree_closest_point<Index, RealT, N> result{radius *
                                                                 radius};
  tf::implementation::tree_closest_point_using_sort_by_level(
      tree.nodes(), tree.ids(), aabb_metric, closest_point_f, result);
  return result.point;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::mod_tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     RealT radius) {
  tf::implementation::tree_closest_point<Index, RealT, N> result{radius *
                                                                 radius};
  tf::implementation::tree_closest_point_using_sort_by_level(
      tree.main_tree().nodes(), tree.main_tree().ids(), aabb_metric,
      closest_point_f, result);
  tf::implementation::tree_closest_point_using_sort_by_level(
      tree.delta_tree().nodes(), tree.delta_tree().ids(), aabb_metric,
      closest_point_f, result);
  return result.point;
}

/// @ingroup spatial_queries
/// @brief Perform a nearest-point spatial query against a single tree structure.
///
/// Traverses the tree to find the closest primitive to an implicit query point,
/// based on user-provided AABB distance and primitive distance callbacks.
/// The tree is searched using a top-k sorted traversal strategy.
///
/// @param tree The spatial tree to query.
/// @param aabb_metric A function that estimates the distance to a node's AABB.
///                    Signature: `(const tf::aabb<RealT, N>& aabb) -> tf::aabb_metrics<RealT>`
/// @param closest_point_f A function that evaluates the true distance to a primitive.
///                        Signature: `(Index id) -> tf::closest_point<RealT, N>`
/// @param radius The search radius for the query
///
/// @return tf::tree_closest_point<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     RealT radius) {
  return nearness_search(strategy::top_k_sorted, tree, aabb_metric,
                         closest_point_f, radius);
}

/// @ingroup spatial_queries
/// @brief Perform a nearest-point spatial query against a single tree structure.
///
/// Traverses the tree to find the closest primitive to an implicit query point,
/// based on user-provided AABB distance and primitive distance callbacks.
/// The tree is searched using a top-k sorted traversal strategy.
///
/// @param tree The spatial tree to query.
/// @param aabb_metric A function that estimates the distance to a node's AABB.
///                    Signature: `(const tf::aabb<RealT, N>& aabb) -> tf::aabb_metrics<RealT>`
/// @param closest_point_f A function that evaluates the true distance to a primitive.
///                        Signature: `(Index id) -> tf::closest_point<RealT, N>`
/// @param radius The search radius for the query
///
/// @return tf::tree_closest_point<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::mod_tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     RealT radius) {
  return nearness_search(strategy::top_k_sorted, tree, aabb_metric,
                         closest_point_f, radius);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::priority_queue_t,
                     const tf::tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     RealT radius) {
  tf::implementation::tree_closest_point<Index, RealT, N> result{radius *
                                                                 radius};
  tf::implementation::tree_closest_point_using_heap(
      tree.nodes(), tree.ids(), aabb_metric, closest_point_f, result);
  return result.point;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::priority_queue_t,
                     const tf::mod_tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     RealT radius) {
  tf::implementation::tree_closest_point<Index, RealT, N> result{radius *
                                                                 radius};
  tf::implementation::tree_closest_point_using_heap(
      tree.main_tree().nodes(), tree.main_tree().ids(), aabb_metric,
      closest_point_f, result);
  tf::implementation::tree_closest_point_using_heap(
      tree.delta_tree().nodes(), tree.delta_tree().ids(), aabb_metric,
      closest_point_f, result);
  return result.point;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT radius) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{radius *
                                                                      radius};
  tf::implementation::tree_tree_proximity_sort(tree0, tree1, aabb_metrics_f,
                                               closest_points_f, result);
  return result.points;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT radius) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{radius *
                                                                      radius};

  tf::implementation::tree_tree_proximity_sort(
      tree0.main_tree(), tree1.main_tree(), aabb_metrics_f, closest_points_f,
      result);
  tf::implementation::tree_tree_proximity_sort(
      tree0.main_tree(), tree1.delta_tree(), aabb_metrics_f, closest_points_f,
      result);
  tf::implementation::tree_tree_proximity_sort(
      tree0.delta_tree(), tree1.main_tree(), aabb_metrics_f, closest_points_f,
      result);
  tf::implementation::tree_tree_proximity_sort(
      tree0.delta_tree(), tree1.delta_tree(), aabb_metrics_f, closest_points_f,
      result);
  return result.points;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT radius) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{radius *
                                                                      radius};

  tf::implementation::tree_tree_proximity_sort(
      tree0.main_tree(), tree1, aabb_metrics_f, closest_points_f, result);
  tf::implementation::tree_tree_proximity_sort(
      tree0.delta_tree(), tree1, aabb_metrics_f, closest_points_f, result);

  return result.points;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT radius) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{radius *
                                                                      radius};

  tf::implementation::tree_tree_proximity_sort(
      tree0, tree1.main_tree(), aabb_metrics_f, closest_points_f, result);
  tf::implementation::tree_tree_proximity_sort(
      tree0, tree1.delta_tree(), aabb_metrics_f, closest_points_f, result);

  return result.points;
}

/// @ingroup spatial_queries
/// @brief Perform a nearest-point spatial query between two tree structures.
///
/// Traverses both trees in tandem to find the closest pair of primitives between them,
/// using user-provided AABB distance and primitive distance callbacks. 
///
/// @param tree0 The first spatial tree to query.
/// @param tree1 The second spatial tree to query.
/// @param aabb_metrics_f A function that estimates the distances between two AABBs.
///                       Signature: `(const tf::aabb<RealT, N>& a, const tf::aabb<RealT, N>& b) -> tf::aabb_metrics<RealT>`
/// @param closest_points_f A function that evaluates the true distance between a pair of primitives.
///                         Signature: `(Index id0, Index id1) -> tf::closest_point_pair<RealT, N>`
/// @param radius The search radius for the query
///
/// @return tf::tree_closest_point_pair<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT radius) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f, radius);
}

/// @ingroup spatial_queries
/// @brief Perform a nearest-point spatial query between two tree structures.
///
/// Traverses both trees in tandem to find the closest pair of primitives between them,
/// using user-provided AABB distance and primitive distance callbacks. 
///
/// @param tree0 The first spatial tree to query.
/// @param tree1 The second spatial tree to query.
/// @param aabb_metrics_f A function that estimates the distances between two AABBs.
///                       Signature: `(const tf::aabb<RealT, N>& a, const tf::aabb<RealT, N>& b) -> tf::aabb_metrics<RealT>`
/// @param closest_points_f A function that evaluates the true distance between a pair of primitives.
///                         Signature: `(Index id0, Index id1) -> tf::closest_point_pair<RealT, N>`
/// @param radius The search radius for the query
///
/// @return tf::tree_closest_point_pair<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT radius) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f, radius);
}

/// @ingroup spatial_queries
/// @brief Perform a nearest-point spatial query between two tree structures.
///
/// Traverses both trees in tandem to find the closest pair of primitives between them,
/// using user-provided AABB distance and primitive distance callbacks. 
///
/// @param tree0 The first spatial tree to query.
/// @param tree1 The second spatial tree to query.
/// @param aabb_metrics_f A function that estimates the distances between two AABBs.
///                       Signature: `(const tf::aabb<RealT, N>& a, const tf::aabb<RealT, N>& b) -> tf::aabb_metrics<RealT>`
/// @param closest_points_f A function that evaluates the true distance between a pair of primitives.
///                         Signature: `(Index id0, Index id1) -> tf::closest_point_pair<RealT, N>`
/// @param radius The search radius for the query
///
/// @return tf::tree_closest_point_pair<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT radius) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f, radius);
}

/// @ingroup spatial_queries
/// @brief Perform a nearest-point spatial query between two tree structures.
///
/// Traverses both trees in tandem to find the closest pair of primitives between them,
/// using user-provided AABB distance and primitive distance callbacks. 
///
/// @param tree0 The first spatial tree to query.
/// @param tree1 The second spatial tree to query.
/// @param aabb_metrics_f A function that estimates the distances between two AABBs.
///                       Signature: `(const tf::aabb<RealT, N>& a, const tf::aabb<RealT, N>& b) -> tf::aabb_metrics<RealT>`
/// @param closest_points_f A function that evaluates the true distance between a pair of primitives.
///                         Signature: `(Index id0, Index id1) -> tf::closest_point_pair<RealT, N>`
/// @param radius The search radius for the query
///
/// @return tf::tree_closest_point_pair<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT radius) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f, radius);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::priority_queue_t,
                     const tf::tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT radius) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{radius *
                                                                      radius};
  tf::implementation::tree_tree_proximity_heap(tree0, tree1, aabb_metrics_f,
                                               closest_points_f, result);
  return result.points;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::priority_queue_t,
                     const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT radius) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{radius *
                                                                      radius};

  tf::implementation::tree_tree_proximity_heap(
      tree0.main_tree(), tree1.main_tree(), aabb_metrics_f, closest_points_f,
      result);
  tf::implementation::tree_tree_proximity_heap(
      tree0.main_tree(), tree1.delta_tree(), aabb_metrics_f, closest_points_f,
      result);
  tf::implementation::tree_tree_proximity_heap(
      tree0.delta_tree(), tree1.main_tree(), aabb_metrics_f, closest_points_f,
      result);
  tf::implementation::tree_tree_proximity_heap(
      tree0.delta_tree(), tree1.delta_tree(), aabb_metrics_f, closest_points_f,
      result);
  return result.points;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::priority_queue_t,
                     const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT radius) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{radius *
                                                                      radius};

  tf::implementation::tree_tree_proximity_heap(
      tree0.main_tree(), tree1, aabb_metrics_f, closest_points_f, result);
  tf::implementation::tree_tree_proximity_heap(
      tree0.delta_tree(), tree1, aabb_metrics_f, closest_points_f, result);

  return result.points;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::priority_queue_t,
                     const tf::tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT radius) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{radius *
                                                                      radius};

  tf::implementation::tree_tree_proximity_heap(
      tree0, tree1.main_tree(), aabb_metrics_f, closest_points_f, result);
  tf::implementation::tree_tree_proximity_heap(
      tree0, tree1.delta_tree(), aabb_metrics_f, closest_points_f, result);

  return result.points;
}

// knn

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     tf::tree_knn<RandomIt> &knn) {
  tf::implementation::tree_closest_point_using_sort_by_level(
      tree.nodes(), tree.ids(), aabb_metric, closest_point_f, knn);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::mod_tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     tf::tree_knn<RandomIt> &knn) {
  tf::implementation::tree_closest_point_using_sort_by_level(
      tree.main_tree().nodes(), tree.main_tree().ids(), aabb_metric,
      closest_point_f, knn);
  tf::implementation::tree_closest_point_using_sort_by_level(
      tree.delta_tree().nodes(), tree.delta_tree().ids(), aabb_metric,
      closest_point_f, knn);
}

/// @ingroup spatial_queries
/// @brief Perform a knn spatial query against a single tree structure.
///
/// Traverses the tree to find the k nearest primitives to an implicit query point,
/// based on user-provided AABB distance and primitive distance callbacks.
/// The tree is searched using a top-k sorted traversal strategy.
///
/// @param tree The spatial tree to query.
/// @param aabb_metric A function that estimates the distance to a node's AABB.
///                    Signature: `(const tf::aabb<RealT, N>& aabb) -> RealT`
/// @param closest_point_f A function that evaluates the true distance to a primitive.
///                        Signature: `(Index id) -> tf::closest_point<RealT, N>`
/// @param knn The accumulator `tf::tree_knn` for the query
///
/// @return tf::tree_closest_point<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(const tf::tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     tf::tree_knn<RandomIt> &knn) {
  return nearness_search(strategy::top_k_sorted, tree, aabb_metric,
                         closest_point_f, knn);
}

/// @ingroup spatial_queries
/// @brief Perform a knn spatial query against a single tree structure.
///
/// Traverses the tree to find the k nearest primitives to an implicit query point,
/// based on user-provided AABB distance and primitive distance callbacks.
/// The tree is searched using a top-k sorted traversal strategy.
///
/// @param tree The spatial tree to query.
/// @param aabb_metric A function that estimates the distance to a node's AABB.
///                    Signature: `(const tf::aabb<RealT, N>& aabb) -> RealT`
/// @param closest_point_f A function that evaluates the true distance to a primitive.
///                        Signature: `(Index id) -> tf::closest_point<RealT, N>`
/// @param knn The accumulator `tf::tree_knn` for the query
///
/// @return tf::tree_closest_point<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(const tf::mod_tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     tf::tree_knn<RandomIt> &knn) {
  return nearness_search(strategy::top_k_sorted, tree, aabb_metric,
                         closest_point_f, knn);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(strategy::priority_queue_t,
                     const tf::tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     tf::tree_knn<RandomIt> &knn) {
  tf::implementation::tree_closest_point_using_heap(
      tree.nodes(), tree.ids(), aabb_metric, closest_point_f, knn);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(strategy::priority_queue_t,
                     const tf::mod_tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     tf::tree_knn<RandomIt> &knn) {
  tf::implementation::tree_closest_point_using_heap(
      tree.main_tree().nodes(), tree.main_tree().ids(), aabb_metric,
      closest_point_f, knn);
  tf::implementation::tree_closest_point_using_heap(
      tree.delta_tree().nodes(), tree.delta_tree().ids(), aabb_metric,
      closest_point_f, knn);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     tree_knn<RandomIt> &knn) {
  tf::implementation::tree_tree_proximity_sort(tree0, tree1, aabb_metrics_f,
                                               closest_points_f, knn);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     tree_knn<RandomIt> &knn) {
  tf::implementation::tree_tree_proximity_sort(
      tree0.main_tree(), tree1.main_tree(), aabb_metrics_f, closest_points_f,
      knn);
  tf::implementation::tree_tree_proximity_sort(
      tree0.main_tree(), tree1.delta_tree(), aabb_metrics_f, closest_points_f,
      knn);
  tf::implementation::tree_tree_proximity_sort(
      tree0.delta_tree(), tree1.main_tree(), aabb_metrics_f, closest_points_f,
      knn);
  tf::implementation::tree_tree_proximity_sort(
      tree0.delta_tree(), tree1.delta_tree(), aabb_metrics_f, closest_points_f,
      knn);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     tree_knn<RandomIt> &knn) {
  tf::implementation::tree_tree_proximity_sort(
      tree0, tree1.main_tree(), aabb_metrics_f, closest_points_f, knn);
  tf::implementation::tree_tree_proximity_sort(
      tree0, tree1.delta_tree(), aabb_metrics_f, closest_points_f, knn);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     tree_knn<RandomIt> &knn) {
  tf::implementation::tree_tree_proximity_sort(
      tree0.main_tree(), tree1, aabb_metrics_f, closest_points_f, knn);
  tf::implementation::tree_tree_proximity_sort(
      tree0.delta_tree(), tree1, aabb_metrics_f, closest_points_f, knn);
}

/// @ingroup spatial_queries
/// @brief Perform a knn spatial query between two tree structures.
///
/// Traverses both trees in tandem to find the k nearest pairs of primitives between them,
/// using user-provided AABB distance and primitive distance callbacks. 
///
/// @param tree0 The first spatial tree to query.
/// @param tree1 The second spatial tree to query.
/// @param aabb_metrics_f A function that estimates the distances between two AABBs.
///                       Signature: `(const tf::aabb<RealT, N>& a, const tf::aabb<RealT, N>& b) -> tf::aabb_metrics<RealT>`
/// @param closest_points_f A function that evaluates the true distance between a pair of primitives.
///                         Signature: `(Index id0, Index id1) -> tf::closest_point_pair<RealT, N>`
/// @param knn The accumulator `tf::tree_knn` for the query
///
/// @return tf::tree_closest_point_pair<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(const tf::tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     tree_knn<RandomIt> &knn) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f, knn);
}

/// @ingroup spatial_queries
/// @brief Perform a knn spatial query between two tree structures.
///
/// Traverses both trees in tandem to find the k nearest pairs of primitives between them,
/// using user-provided AABB distance and primitive distance callbacks. 
///
/// @param tree0 The first spatial tree to query.
/// @param tree1 The second spatial tree to query.
/// @param aabb_metrics_f A function that estimates the distances between two AABBs.
///                       Signature: `(const tf::aabb<RealT, N>& a, const tf::aabb<RealT, N>& b) -> tf::aabb_metrics<RealT>`
/// @param closest_points_f A function that evaluates the true distance between a pair of primitives.
///                         Signature: `(Index id0, Index id1) -> tf::closest_point_pair<RealT, N>`
/// @param knn The accumulator `tf::tree_knn` for the query
///
/// @return tf::tree_closest_point_pair<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     tree_knn<RandomIt> &knn) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f, knn);
}

/// @ingroup spatial_queries
/// @brief Perform a knn spatial query between two tree structures.
///
/// Traverses both trees in tandem to find the k nearest pairs of primitives between them,
/// using user-provided AABB distance and primitive distance callbacks. 
///
/// @param tree0 The first spatial tree to query.
/// @param tree1 The second spatial tree to query.
/// @param aabb_metrics_f A function that estimates the distances between two AABBs.
///                       Signature: `(const tf::aabb<RealT, N>& a, const tf::aabb<RealT, N>& b) -> tf::aabb_metrics<RealT>`
/// @param closest_points_f A function that evaluates the true distance between a pair of primitives.
///                         Signature: `(Index id0, Index id1) -> tf::closest_point_pair<RealT, N>`
/// @param knn The accumulator `tf::tree_knn` for the query
///
/// @return tf::tree_closest_point_pair<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     tree_knn<RandomIt> &knn) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f, knn);
}

/// @ingroup spatial_queries
/// @brief Perform a knn spatial query between two tree structures.
///
/// Traverses both trees in tandem to find the k nearest pairs of primitives between them,
/// using user-provided AABB distance and primitive distance callbacks. 
///
/// @param tree0 The first spatial tree to query.
/// @param tree1 The second spatial tree to query.
/// @param aabb_metrics_f A function that estimates the distances between two AABBs.
///                       Signature: `(const tf::aabb<RealT, N>& a, const tf::aabb<RealT, N>& b) -> tf::aabb_metrics<RealT>`
/// @param closest_points_f A function that evaluates the true distance between a pair of primitives.
///                         Signature: `(Index id0, Index id1) -> tf::closest_point_pair<RealT, N>`
/// @param knn The accumulator `tf::tree_knn` for the query
///
/// @return tf::tree_closest_point_pair<Index, RealT, N>.
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(const tf::tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     tree_knn<RandomIt> &knn) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f, knn);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(strategy::priority_queue_t,
                     const tf::tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     tree_knn<RandomIt> &knn) {
  tf::implementation::tree_tree_proximity_heap(tree0, tree1, aabb_metrics_f,
                                               closest_points_f, knn);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(strategy::priority_queue_t,
                     const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     tree_knn<RandomIt> &knn) {
  tf::implementation::tree_tree_proximity_heap(
      tree0.main_tree(), tree1.main_tree(), aabb_metrics_f, closest_points_f,
      knn);
  tf::implementation::tree_tree_proximity_heap(
      tree0.main_tree(), tree1.delta_tree(), aabb_metrics_f, closest_points_f,
      knn);
  tf::implementation::tree_tree_proximity_heap(
      tree0.delta_tree(), tree1.main_tree(), aabb_metrics_f, closest_points_f,
      knn);
  tf::implementation::tree_tree_proximity_heap(
      tree0.delta_tree(), tree1.delta_tree(), aabb_metrics_f, closest_points_f,
      knn);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(strategy::priority_queue_t,
                     const tf::tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     tree_knn<RandomIt> &knn) {
  tf::implementation::tree_tree_proximity_heap(
      tree0, tree1.main_tree(), aabb_metrics_f, closest_points_f, knn);
  tf::implementation::tree_tree_proximity_heap(
      tree0, tree1.delta_tree(), aabb_metrics_f, closest_points_f, knn);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(strategy::priority_queue_t,
                     const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     tree_knn<RandomIt> &knn) {
  tf::implementation::tree_tree_proximity_heap(
      tree0.main_tree(), tree1, aabb_metrics_f, closest_points_f, knn);
  tf::implementation::tree_tree_proximity_heap(
      tree0.delta_tree(), tree1, aabb_metrics_f, closest_points_f, knn);
}

} // namespace tf
