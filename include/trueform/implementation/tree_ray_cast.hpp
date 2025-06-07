/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "../epsilon_inverse.hpp"
#include "../intersect_status.hpp"
#include "../ray.hpp"
#include "../sequence_range.hpp"
#include "../small_buffer.hpp"
#include "../tree.hpp"
#include "./ray_aabb_check.hpp"
#include "./tree_ray_info.hpp"
#include <cstdint>

namespace tf::implementation {
template <typename Index, typename RealT, std::size_t Dims, typename InfoT,
          typename F>
auto tree_ray_cast(const tf::tree<Index, RealT, Dims> &tree,
                   const tf::ray<RealT, Dims> &ray,
                   tf::implementation::tree_ray_info<Index, InfoT> &result,
                   const F &intersect_f) {
  if (!tree.nodes().size())
    return;
  const auto &nodes = tree.nodes();
  const auto &ids = tree.ids();
  tf::vector<RealT, Dims> ray_inv_dir;
  tf::small_buffer<Index, 256> stack;
  stack.push_back(0);
  //
  std::array<std::int8_t, Dims> dir_sign;
  Index inv_dir_size = ray_inv_dir.size();
  for (std::size_t i = 0; i < Dims; ++i) {
    // set the direction sign
    dir_sign[i] = ray.direction[i] < 0;
    // precompute inverse of the direction
    ray_inv_dir[i] = tf::epsilon_inverse(ray.direction[i]);
  }

  RealT t_min = std::numeric_limits<RealT>::max();
  RealT t_max = -std::numeric_limits<RealT>::max();

  auto min_t = result.min_t();
  auto max_t = result.max_t();
  while (stack.size()) {

    auto current_i = stack.back();
    stack.pop_back();
    const auto &node = nodes[current_i];
    auto hit = implementation::ray_aabb_check(ray, ray_inv_dir, node.aabb,
                                              t_min, t_max, min_t, max_t) ==
               tf::intersect_status::intersection;
    if (hit) {
      const auto &data = node.get_data();
      if (!node.is_leaf()) {
        auto nexts = tf::make_sequence_range(data[0], data[0] + data[1]);
        auto is_negative = dir_sign[node.axis];
        if (!is_negative) {
          std::reverse_copy(nexts.begin(), nexts.end(),
                            std::back_inserter(stack));
        } else {
          std::copy(nexts.begin(), nexts.end(), std::back_inserter(stack));
        }
      } else {
        for (const auto &id : tf::make_range(ids.begin(), data[0]), data[1])
          max_t = result.update(id, intersect_f(ray, id));
      }
    }
  }
}

} // namespace tf::implementation
