/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "../small_buffer.hpp"
#include "../tree.hpp"
namespace tf::implementation {
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename Result>
auto tree_tree_proximity_sort(const tf::tree<Index, RealT, N> &tree0,
                              const tf::tree<Index, RealT, N> &tree1,
                              const F0 &aabb_dists_f, const F1 &closest_pts,
                              Result &result) {
  if (!tree0.nodes().size() || !tree1.nodes().size())
    return;
  struct holder_t {
    RealT min2;
    RealT min_max2;
    Index id0;
    Index id1;
  };
  tf::small_buffer<holder_t, 256> stack;
  stack.push_back(holder_t{std::numeric_limits<float>::max(),
                           std::numeric_limits<float>::max(), 0, 0});

  RealT aabb_min2 = result.metric();
  RealT aabb_min_max2 = result.metric();
  const auto &nodes0 = tree0.nodes();
  const auto &nodes1 = tree1.nodes();
  const auto &ids0 = tree0.ids();
  const auto &ids1 = tree1.ids();

  auto dispatch = [&](Index last_id) {
    std::sort(stack.begin() + std::max(last_id - 4, 0), stack.end(),
              [](const auto &x, const auto &y) {
                return std::make_pair(x.min2, x.min_max2) >
                       std::make_pair(y.min2, y.min_max2);
              });
  };

  auto push_f = [&](Index id0, Index id1) {
    auto ds2 = aabb_dists_f(tree0.nodes()[id0].aabb, tree1.nodes()[id1].aabb);
    if (ds2.min_d2 > result.metric() || ds2.min_d2 > aabb_min_max2)
      return;
    aabb_min2 = std::min(ds2.min_d2, aabb_min2);
    aabb_min_max2 = std::min(ds2.min_max_d2, aabb_min_max2);
    stack.push_back({ds2.min_d2, ds2.min_max_d2, id0, id1});
  };

  while (stack.size()) {
    auto candidate = stack.back();
    stack.pop_back();
    if (candidate.min2 > result.metric() || candidate.min2 > aabb_min_max2)
      continue;
    Index last_id = stack.size();
    const auto &node0 = nodes0[candidate.id0];
    const auto &node1 = nodes1[candidate.id1];
    const auto &data0 = node0.get_data();
    const auto &data1 = node1.get_data();

    if (!node0.is_leaf() && !node1.is_leaf()) {
      for (auto n_id0 = data0[0]; n_id0 < data0[0] + data0[1]; ++n_id0)
        for (auto n_id1 = data1[0]; n_id1 < data1[0] + data1[1]; ++n_id1)
          push_f(n_id0, n_id1);
      dispatch(last_id);
    } else if (!node0.is_leaf()) {
      for (auto n_id0 = data0[0]; n_id0 < data0[0] + data0[1]; ++n_id0)
        push_f(n_id0, candidate.id1);
      dispatch(last_id);

    } else if (!node1.is_leaf()) {
      for (auto n_id1 = data1[0]; n_id1 < data1[0] + data1[1]; ++n_id1)
        push_f(candidate.id0, n_id1);
      dispatch(last_id);
    } else {
      for (auto n_id0 = data0[0]; n_id0 < data0[0] + data0[1]; ++n_id0)
        for (auto n_id1 = data1[0]; n_id1 < data1[0] + data1[1]; ++n_id1) {
          if (result.update(std::make_pair(ids0[n_id0], ids1[n_id1]),
                            closest_pts(ids0[n_id0], ids1[n_id1])))
            return;
        }
    }
  }
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename Result>
auto tree_tree_proximity_heap(const tf::tree<Index, RealT, N> &tree0,
                              const tf::tree<Index, RealT, N> &tree1,
                              const F0 &aabb_dists_f, const F1 &closest_pts,
                              Result &result) {
  if (!tree0.nodes().size() || !tree1.nodes().size())
    return;
  struct holder_t {
    RealT min2;
    RealT min_max2;
    Index id0;
    Index id1;
  };
  tf::small_buffer<holder_t, 256> heap;
  heap.push_back(holder_t{std::numeric_limits<float>::max(),
                          std::numeric_limits<float>::max(), 0, 0});

  RealT aabb_min2 = result.metric();
  RealT aabb_min_max2 = result.metric();
  const auto &nodes0 = tree0.nodes();
  const auto &nodes1 = tree1.nodes();
  const auto &ids0 = tree0.ids();
  const auto &ids1 = tree1.ids();

  auto compare = [](const auto &x, const auto &y) {
    return std::make_pair(x.min2, x.min_max2) >
           std::make_pair(y.min2, y.min_max2);
  };

  auto push_f = [&](Index id0, Index id1) {
    auto ds2 = aabb_dists_f(tree0.nodes()[id0].aabb, tree1.nodes()[id1].aabb);
    if (ds2.min_d2 > result.metric() || ds2.min_d2 > aabb_min_max2)
      return;
    aabb_min2 = std::min(ds2.min_d2, aabb_min2);
    aabb_min_max2 = std::min(ds2.min_max_d2, aabb_min_max2);
    heap.push_back({ds2.min_d2, ds2.min_max_d2, id0, id1});
    std::push_heap(heap.begin(), heap.end(), compare);
  };

  while (heap.size()) {
    std::pop_heap(heap.begin(), heap.end(), compare);
    auto candidate = heap.back();
    heap.pop_back();
    if (candidate.min2 > result.metric() || candidate.min2 > aabb_min_max2)
      continue;
    const auto &node0 = nodes0[candidate.id0];
    const auto &node1 = nodes1[candidate.id1];
    const auto &data0 = node0.get_data();
    const auto &data1 = node1.get_data();

    if (!node0.is_leaf() && !node1.is_leaf()) {
      for (auto n_id0 = data0[0]; n_id0 < data0[0] + data0[1]; ++n_id0)
        for (auto n_id1 = data1[0]; n_id1 < data1[0] + data1[1]; ++n_id1)
          push_f(n_id0, n_id1);
    } else if (!node0.is_leaf()) {
      for (auto n_id0 = data0[0]; n_id0 < data0[0] + data0[1]; ++n_id0)
        push_f(n_id0, candidate.id1);

    } else if (!node1.is_leaf()) {
      for (auto n_id1 = data1[0]; n_id1 < data1[0] + data1[1]; ++n_id1)
        push_f(candidate.id0, n_id1);
    } else {
      for (auto n_id0 = data0[0]; n_id0 < data0[0] + data0[1]; ++n_id0)
        for (auto n_id1 = data1[0]; n_id1 < data1[0] + data1[1]; ++n_id1) {
          if (result.update(std::make_pair(ids0[n_id0], ids1[n_id1]),
                            closest_pts(ids0[n_id0], ids1[n_id1])))
            return;
        }
    }
  }
}

} // namespace tf::implementation
