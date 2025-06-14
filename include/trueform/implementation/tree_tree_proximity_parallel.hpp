/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "../small_buffer.hpp"
#include "../tree.hpp"
#include "./local_tree_metric_result.hpp"
#include "tbb/task_group.h"
#include <utility>
namespace tf::implementation {
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename T>
struct tree_tree_proximity_params {
  const tf::tree<Index, RealT, N> &tree0;
  const tf::tree<Index, RealT, N> &tree1;
  const F0 &aabb_dists_f;
  const F1 &closest_pts;
  local_tree_metric_result<T> &result;
};

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename T>
auto tree_tree_proximity_parallel2(
    Index id0, Index id1,
    const tree_tree_proximity_params<Index, RealT, N, F0, F1, T> &params) {

  struct holder_t {
    RealT min2;
    RealT min_max2;
    Index id0;
    Index id1;
  };

  const auto &nodes0 = params.tree0.nodes();
  const auto &nodes1 = params.tree1.nodes();
  const auto &ids0 = params.tree0.ids();
  const auto &ids1 = params.tree1.ids();

  tf::small_buffer<holder_t, 256> stack;

  auto push_f = [&](Index id0, Index id1) {
    const auto &node0 = nodes0[id0];
    const auto &node1 = nodes1[id1];
    auto [aabb_min, aabb_max] = params.aabb_dists_f(node0.aabb, node1.aabb);
    if (params.result.reject_aabbs(aabb_min))
      return;
    params.result.update_aabb_min(aabb_min);
    params.result.update_aabb_max(aabb_max);
    stack.push_back({aabb_min, aabb_max, id0, id1});
  };

  auto dispatch = [&](Index last_id) {
    std::sort(stack.begin() + std::max(last_id - 4, 0), stack.end(),
              [](const auto &x, const auto &y) {
                return std::make_pair(x.min2, x.min_max2) >
                       std::make_pair(y.min2, y.min_max2);
              });
  };

  push_f(id0, id1);

  while (stack.size()) {
    auto candidate = stack.back();
    stack.pop_back();
    if (params.result.reject_aabbs(candidate.min2))
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
          if (params.result.update(std::make_pair(ids0[n_id0], ids1[n_id1]),
                                   params.closest_pts(ids0[n_id0], ids1[n_id1])))
            return;
        }
    }
  }
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename T>
auto tree_tree_proximity_parallel(
    Index id0, Index id1,
    const tree_tree_proximity_params<Index, RealT, N, F0, F1, T> &params,
    int depth = 0) {

  const auto &nodes0 = params.tree0.nodes();
  const auto &nodes1 = params.tree1.nodes();
  const auto &ids0 = params.tree0.ids();
  const auto &ids1 = params.tree1.ids();
  const auto &node0 = nodes0[id0];
  const auto &node1 = nodes1[id1];
  const auto &data0 = node0.get_data();
  const auto &data1 = node1.get_data();

  auto [aabb_min, aabb_max] = params.aabb_dists_f(node0.aabb, node1.aabb);
  if (params.result.reject_aabbs(aabb_min))
    return;
  params.result.update_aabb_min(aabb_min);
  params.result.update_aabb_max(aabb_max);

  tbb::task_group tg;
  auto dispatch_f = [&](Index id0, Index id1) {
    tg.run([&, id0, id1] {
      tree_tree_proximity_parallel(id0, id1, params, depth + 1);
    });
  };

  if (!node0.is_leaf() && !node1.is_leaf()) {
    for (auto n_id0 = data0[0]; n_id0 < data0[0] + data0[1]; ++n_id0)
      for (auto n_id1 = data1[0]; n_id1 < data1[0] + data1[1]; ++n_id1)
        dispatch_f(n_id0, n_id1);
  } else if (!node0.is_leaf()) {
    for (auto n_id0 = data0[0]; n_id0 < data0[0] + data0[1]; ++n_id0)
      dispatch_f(n_id0, id1);

  } else if (!node1.is_leaf()) {
    for (auto n_id1 = data1[0]; n_id1 < data1[0] + data1[1]; ++n_id1)
      dispatch_f(id0, n_id1);
  } else {
    for (auto n_id0 = data0[0]; n_id0 < data0[0] + data0[1]; ++n_id0)
      for (auto n_id1 = data1[0]; n_id1 < data1[0] + data1[1]; ++n_id1) {
        if (params.result.update(std::make_pair(n_id0, n_id1),
                                 params.closest_pts(ids0[n_id0], ids1[n_id1])))
          return;
      }
  }

  tg.wait();
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename T>
auto tree_tree_proximity_parallel(const tf::tree<Index, RealT, N> &tree0,
                                  const tf::tree<Index, RealT, N> &tree1,
                                  const F0 &aabb_dists_f, const F1 &closest_pts,
                                  local_tree_metric_result<T> &result) {
  tree_tree_proximity_params<Index, RealT, N, F0, F1, T> params{
      tree0, tree1, aabb_dists_f, closest_pts, result};
  tree_tree_proximity_parallel(0, 0, params);
}

} // namespace tf::implementation
