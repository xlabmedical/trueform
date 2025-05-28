/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./implementation/tree_closest_point_using_sort_by_level.hpp"
#include "./implementation/tree_tree_proximity.hpp"
#include "./tree.hpp"

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
auto nearness_search(const tf::tree<Index, RealT, N> &tree,
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
auto nearness_search(const tf::tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
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

// nearness in search_radius

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     RealT search_radius) {
  tf::implementation::tree_closest_point<Index, RealT, N> result{search_radius *
                                                                 search_radius};
  tf::implementation::tree_closest_point_using_sort_by_level(
      tree.nodes(), tree.ids(), aabb_metric, closest_point_f, result);
  return result.point;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     RealT search_radius) {
  return nearness_search(strategy::top_k_sorted, tree, aabb_metric,
                         closest_point_f, search_radius);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::priority_queue_t,
                     const tf::tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     RealT search_radius) {
  tf::implementation::tree_closest_point<Index, RealT, N> result{search_radius *
                                                                 search_radius};
  tf::implementation::tree_closest_point_using_heap(
      tree.nodes(), tree.ids(), aabb_metric, closest_point_f, result);
  return result.point;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::top_k_sorted_t,
                     const tf::tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT search_radius) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{
      search_radius * search_radius};
  tf::implementation::tree_tree_proximity_sort(tree0, tree1, aabb_metrics_f,
                                               closest_points_f, result);
  return result.points;
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT search_radius) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f, search_radius);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(strategy::priority_queue_t,
                     const tf::tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT search_radius) {
  tf::implementation::tree_closest_point_pair<Index, RealT, N> result{
      search_radius * search_radius};
  tf::implementation::tree_tree_proximity_heap(tree0, tree1, aabb_metrics_f,
                                               closest_points_f, result);
  return result.points;
}

} // namespace tf
