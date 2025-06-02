/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./closest_point_pair.hpp"
#include <utility>

namespace tf {

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
