/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./closest_point.hpp"

namespace tf {

template <typename Index, typename RealT, std::size_t Dims>
struct tree_closest_point {
  static constexpr Index no_id = -1;
  Index element{no_id};
  tf::closest_point<RealT, Dims> point;

  operator bool() const { return element != no_id; }
};
} // namespace tf
