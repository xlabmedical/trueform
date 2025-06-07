/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "../aabb.hpp"
#include "../intersect_status.hpp"
#include "../ray.hpp"
#include "../vector_like.hpp"
#include <algorithm>

namespace tf::implementation {

template <typename RealT, std::size_t Dims, typename T>
auto ray_aabb_check(const tf::ray<RealT, Dims> &ray,
                            const tf::vector_like<Dims, T> &ray_dir_inv,
                            const tf::aabb<RealT, Dims> &bounding_box,
                            RealT &t_min, RealT &t_max, RealT min_t,
                            RealT max_t) {
  auto &&min = bounding_box.min();
  auto &&max = bounding_box.max();
  for (std::size_t i = 0; i < Dims; ++i) {
    auto min_i = min[i];
    auto max_i = max[i];
    if (ray_dir_inv[i] < 0)
      std::swap(min_i, max_i);
    auto t0 = (min_i - ray.origin[i]) * ray_dir_inv[i];
    auto t1 = (max_i - ray.origin[i]) * ray_dir_inv[i] *
              (1 + 2 * std::numeric_limits<RealT>::epsilon());
    min_t = std::max(t0, min_t);
    max_t = std::min(t1, max_t);
  }
  if (min_t <= max_t) {
    t_min = min_t;
    t_max = max_t;
    return intersect_status::intersection;
  }
  return intersect_status::none;
}
} // namespace tf::implementation
