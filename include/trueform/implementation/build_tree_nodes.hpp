/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "../aabb_union.hpp"
#include "../buffer.hpp"
#include "../parallel_apply.hpp"
#include "../parallel_iota.hpp"
#include "../span.hpp"
#include "../tree_config.hpp"
#include "../tree_node.hpp"
#include "./max_nodes_in_tree.hpp"
#include "tbb/task_group.h"
#include <algorithm>

namespace tf::implementation {
template <typename Partitioner, typename Index, typename RealT, std::size_t N,
          typename Range0, typename Range1>
auto build_tree_nodes(buffer<tree_node<Index, RealT, N>> &nodes,
                      const Range0 &aabbs, Range1 &ids, Index node_id,
                      Index offset, const tree_config &config, Index depth) {
  // create the bounding box
  {
    nodes[node_id].aabb = aabbs[ids.front()];
    auto it = ids.begin();
    auto end = ids.end();
    while (++it != end)
      aabb_union_inplace(nodes[node_id].aabb, aabbs[*it]);
  }
  Index n_ids = ids.size();
  if (n_ids <= config.leaf_size) {
    nodes[node_id].set_data(offset, ids.size());
    nodes[node_id].set_as_leaf();
    return;
  }

  int max_axis = 0;
  {
    auto diag = nodes[node_id].aabb.diagonal();
    max_axis = std::max_element(diag.begin(), diag.end()) - diag.begin();
  }
  nodes[node_id].axis = max_axis;

  Index n_children = 0;
  auto run_splitting = [&](auto &&f) {
    Index partition_size = (n_ids + config.inner_size - 1) / config.inner_size;
    Index prev_partition = 0;
    Index current_partition = partition_size;
    Index next = config.inner_size * node_id + 1;

    while (prev_partition != n_ids) {
      auto _ids = make_span(ids.begin() + prev_partition,
                            current_partition - prev_partition);
      if (current_partition != n_ids) {
        Partitioner::partition(
            _ids.begin(), _ids.end(), ids.end(), [&](auto id0, auto id1) {
              return (aabbs[id0].max[max_axis] + aabbs[id0].min[max_axis]) / 2 <
                     (aabbs[id1].max[max_axis] + aabbs[id1].min[max_axis]) / 2;
            });
      }
      f(next, prev_partition, _ids);
      ++next;
      ++n_children;
      prev_partition = current_partition;
      current_partition = std::min(current_partition + partition_size, n_ids);
    }
    return n_children;
  };
  if (depth < config.task_cutoff_depth) {
    tbb::task_group tg;
    run_splitting([&](Index next, Index prev_partition, auto _ids) {
      tg.run([&, prev_partition, _ids, next] {
        build_tree_nodes<Partitioner>(nodes, aabbs, _ids, next,
                                      offset + prev_partition, config,
                                      depth + 1);
      });
    });
    tg.wait();
  } else {
    run_splitting([&](Index next, Index prev_partition, auto _ids) {
      build_tree_nodes<Partitioner>(nodes, aabbs, _ids, next,
                                    offset + prev_partition, config, depth + 1);
    });
  }
  nodes[node_id].set_data(config.inner_size * node_id + 1, n_children);
}

template <typename Partitioner, typename Index, typename RealT, std::size_t N,
          typename Range>
auto build_tree_nodes(buffer<tree_node<Index, RealT, N>> &nodes,
                      buffer<Index> &ids, const Range &aabbs,
                      tree_config config) {

  nodes.clear();
  if (!aabbs.size()) {
    ids.clear();
    return;
  }
  nodes.allocate(max_nodes_in_tree(Index(aabbs.size()), config.inner_size,
                                   config.leaf_size));
  tf::parallel_apply(nodes, [](auto &x) { x.set_as_empty(); });
  ids.allocate(aabbs.size());
  tf::parallel_iota(ids, 0);
  return build_tree_nodes<Partitioner>(nodes, aabbs, ids, 0, 0, config, 0);
}
} // namespace tf::implementation
