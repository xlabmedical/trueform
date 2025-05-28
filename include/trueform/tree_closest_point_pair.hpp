/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include <utility>
#include "./closest_point_pair.hpp"

namespace tf {

template <typename Index, typename RealT, std::size_t Dims> struct tree_closest_point_pair {
  static constexpr Index no_id = -1;
  std::pair<Index, Index> elements{no_id, no_id};
  closest_point_pair<RealT, Dims> points;

  operator bool() const { return elements.first != no_id; }
};
} // namespace tf
