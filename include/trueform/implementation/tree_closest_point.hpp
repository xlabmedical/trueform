/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "../tree_closest_point.hpp"

namespace tf::implementation {

template <typename Index, typename RealT, std::size_t Dims>
class tree_closest_point {
public:
  tree_closest_point() = default;
  tree_closest_point(RealT metric) { point.point.metric = metric; }

  auto update(Index c_element, const tf::metric_point<RealT, Dims> &c_point)
      -> void {
    if (c_point.metric < point.point.metric) {
      point.point = c_point;
      point.element = c_element;
    }
  }

  auto metric() { return point.point.metric; }

  tf::tree_closest_point<Index, RealT, Dims> point;
};

} // namespace tf::implementation
