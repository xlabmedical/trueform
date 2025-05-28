/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./aabb.hpp"
#include "./distance.hpp"
#include "./minimal_maximal_distance.hpp"

namespace tf {
template <typename RealT> struct aabb_metrics {
  RealT min_d2;
  RealT min_max_d2;
};

template <typename RealT, std::size_t Dims>
auto make_aabb_metrics(const aabb<RealT, Dims> &aabb0,
                  const aabb<RealT, Dims> &aabb1) {
  return aabb_metrics<RealT>{tf::distance2(aabb0, aabb1),
                               tf::minimal_maximal_distance2(aabb0, aabb1)};
}
} // namespace tf
