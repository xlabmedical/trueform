/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./vector.hpp"

namespace tf {
template <typename RealT, std::size_t Dims> struct closest_point {
  RealT metric;
  vector<RealT, Dims> point;
};

template <typename RealT, std::size_t Dims>
auto make_closest_point(RealT metric, vector<RealT, Dims> point) {
  return closest_point<RealT, Dims>{metric, point};
}
} // namespace tf
