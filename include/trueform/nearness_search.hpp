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

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f) {
  return nearness_search(strategy::top_k_sorted, tree, aabb_metric,
                         closest_point_f);
}

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
auto nearness_search(const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f);
}

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

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     RealT radius) {
  return nearness_search(strategy::top_k_sorted, tree, aabb_metric,
                         closest_point_f, radius);
}

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

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT radius) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f, radius);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::mod_tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT radius) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f, radius);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto nearness_search(const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     RealT radius) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f, radius);
}

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

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(const tf::tree<Index, RealT, N> &tree,
                     const F0 &aabb_metric, const F1 &closest_point_f,
                     tf::tree_knn<RandomIt> &knn) {
  return nearness_search(strategy::top_k_sorted, tree, aabb_metric,
                         closest_point_f, knn);
}

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

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(const tf::tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     tree_knn<RandomIt> &knn) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f, knn);
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename RandomIt>
auto nearness_search(const tf::mod_tree<Index, RealT, N> &tree0,
                     const tf::tree<Index, RealT, N> &tree1,
                     const F0 &aabb_metrics_f, const F1 &closest_points_f,
                     tree_knn<RandomIt> &knn) {
  return nearness_search(strategy::top_k_sorted, tree0, tree1, aabb_metrics_f,
                         closest_points_f, knn);
}

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
