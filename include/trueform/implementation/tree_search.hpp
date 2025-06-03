/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "../buffer.hpp"
#include "../small_buffer.hpp"
#include "../range.hpp"
#include "../tree_node.hpp"

namespace tf::implementation {
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto tree_search(const buffer<tree_node<Index, RealT, N>> &nodes,
                 const buffer<Index> &ids, const F0 &aabb_check,
                 const F1 &leaf_apply) {
  if (!nodes.size())
    return;
  tf::small_buffer<Index, 512> stack;
  stack.push_back(0);
  while (stack.size()) {
    auto current_i = stack.back();
    stack.pop_back();
    const auto &node = nodes[current_i];
    const auto &data = node.get_data();
    if (node.is_leaf()) {
      if (leaf_apply(tf::make_range(ids.begin() + data[0], data[1])))
        return;
      continue;
    }
    auto it = nodes.begin() + data[0];
    auto end = it + data[1];
    auto next_id = data[0];
    while (it != end) {
      if (aabb_check(it->aabb))
        stack.push_back(next_id);
      ++it;
      ++next_id;
    }
  }
}
} // namespace tf::implementation
