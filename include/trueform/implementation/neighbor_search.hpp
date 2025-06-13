/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "../closest_metric_point.hpp"
#include "../distance.hpp"
#include "../nearness_search.hpp"

namespace tf::implementation {

template <std::size_t Dims, typename Policy0, typename T0, typename T1>
auto neighbor_search(const tf::form<Dims, Policy0> &form,
                     const T0 &obj_for_aabb, const T1 &obj) {
  return tf::nearness_search(
      form, [&](const auto &aabb) { return tf::distance2(aabb, obj_for_aabb); },
      [&](const auto &obj_inner) {
        return tf::closest_metric_point(obj_inner, obj);
      });
}
template <std::size_t Dims, typename Policy0, typename T0, typename T1>
auto neighbor_search(const tf::form<Dims, Policy0> &form,
                     const T0 &obj_for_aabb, const T1 &obj,
                     typename Policy0::real_t radius) {
  return tf::nearness_search(
      form, [&](const auto &aabb) { return tf::distance2(aabb, obj_for_aabb); },
      [&](const auto &obj_inner) {
        return tf::closest_metric_point(obj_inner, obj);
      },
      radius);
}

template <std::size_t Dims, typename Policy0, typename T0, typename T1,
          typename RandomIter>
auto neighbor_search(const tf::form<Dims, Policy0> &form,
                     const T0 &obj_for_aabb, const T1 &obj,
                     tf::nearest_neighbors<RandomIter> &knn) {
  return tf::nearness_search(
      form, [&](const auto &aabb) { return tf::distance2(aabb, obj_for_aabb); },
      [&](const auto &obj_inner) {
        return tf::closest_metric_point(obj_inner, obj);
      },
      knn);
}
} // namespace implementation

