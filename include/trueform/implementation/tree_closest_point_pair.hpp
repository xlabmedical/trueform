/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "../tree_closest_point_pair.hpp"

namespace tf::implementation {

template <typename Index, typename RealT, std::size_t Dims>
class tree_closest_point_pair {
public:
  tree_closest_point_pair() = default;
  tree_closest_point_pair(RealT metric) { points.points.metric = metric; }

  auto update(Index c_element0, Index c_element1,
              const tf::closest_point_pair<RealT, Dims> &c_points) -> void {
    if (c_points.metric < points.points.metric) {
      points.elements = {c_element0, c_element1};
      points.points = c_points;
    }
  }

  auto metric() { return points.points.metric; }

  tf::tree_closest_point_pair<Index, RealT, Dims> points;
};
} // namespace tf::implementation
