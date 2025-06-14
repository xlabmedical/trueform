/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "../closest_metric_point.hpp"
#include "../distance.hpp"
#include "../nearness_search.hpp"
#include "./sphere_distance.hpp"

namespace tf::implementation {

template <std::size_t Dims, typename Policy0, typename T0, typename T1>
auto neighbor_search(const tf::form<Dims, Policy0> &form,
                     const T0 &obj_for_aabb, const T1 &obj) {
  using tf::closest_metric_point;
  using tf::distance2;
  return tf::nearness_search(
      form, [&](const auto &aabb) { return distance2(aabb, obj_for_aabb); },
      [&](const auto &obj_inner) {
        return closest_metric_point(obj_inner, obj);
      });
}
template <std::size_t Dims, typename Policy0, typename T0, typename T1>
auto neighbor_search(const tf::form<Dims, Policy0> &form,
                     const T0 &obj_for_aabb, const T1 &obj,
                     typename Policy0::real_t radius) {
  using tf::closest_metric_point;
  using tf::distance2;
  return tf::nearness_search(
      form, [&](const auto &aabb) { return distance2(aabb, obj_for_aabb); },
      [&](const auto &obj_inner) {
        return closest_metric_point(obj_inner, obj);
      },
      radius);
}

template <std::size_t Dims, typename Policy0, typename T0, typename T1,
          typename RandomIter>
auto neighbor_search(const tf::form<Dims, Policy0> &form,
                     const T0 &obj_for_aabb, const T1 &obj,
                     tf::nearest_neighbors<RandomIter> &knn) {
  using tf::closest_metric_point;
  using tf::distance2;
  return tf::nearness_search(
      form, [&](const auto &aabb) { return distance2(aabb, obj_for_aabb); },
      [&](const auto &obj_inner) {
        return closest_metric_point(obj_inner, obj);
      },
      knn);
}

template <std::size_t Dims, typename Policy0, typename T>
auto admissible_neighbor_search(const tf::form<Dims, Policy0> &form,
                                const T &obj, typename Policy0::real_t radius) {
  using tf::closest_metric_point;
  return tf::nearness_search(
      form,
      [&obj](const auto &aabb) {
        return tf::implementation::sphere_distance2(
            tf::make_sphere(aabb.center(), aabb.diagonal().length() / 2), obj);
      },
      [&](const auto &obj_inner) {
        return closest_metric_point(obj_inner, obj);
      },
      radius);
}

template <std::size_t Dims, typename Policy0, typename T, typename RandomIter>
auto admissible_neighbor_search(const tf::form<Dims, Policy0> &form,
                                const T &obj,
                                tf::nearest_neighbors<RandomIter> &knn) {
  using tf::closest_metric_point;
  return tf::nearness_search(
      form,
      [&obj](const auto &aabb) {
        return tf::implementation::sphere_distance2(
            tf::make_sphere(aabb.center(), aabb.diagonal().length() / 2), obj);
      },
      [&](const auto &obj_inner) {
        return closest_metric_point(obj_inner, obj);
      },
      knn);
}
} // namespace tf::implementation
