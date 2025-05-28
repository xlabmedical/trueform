/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./vector.hpp"

namespace tf {
template <typename RealT, std::size_t Dims> struct closest_point_pair {
  RealT metric;
  vector<RealT, Dims> first;
  vector<RealT, Dims> second;
};

template <typename RealT, std::size_t Dims>
auto make_closest_point_pair(RealT metric, vector<RealT, Dims> first,
                             vector<RealT, Dims> second) {
  return closest_point_pair<RealT, Dims>{metric, first, second};
}
} // namespace tf
