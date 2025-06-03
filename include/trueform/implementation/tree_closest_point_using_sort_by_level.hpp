/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "../buffer.hpp"
#include "../range.hpp"
#include "../small_buffer.hpp"
#include "../tree_node.hpp"

namespace tf::implementation {
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename Result>
auto tree_closest_point_using_sort_by_level(
    const buffer<tree_node<Index, RealT, N>> &nodes, const buffer<Index> &ids,
    const F0 &aabb_metric_f, const F1 &closest_point_f, Result &result) {
  if (!nodes.size())
    return;
  using real_type = RealT;
  struct holder_t {
    real_type metric;
    Index id;

    holder_t(Index id, real_type metric) : metric{metric}, id{id} {};
  };

  tf::small_buffer<holder_t, 256> stack;

  auto compare = [](const auto &x, const auto &y) {
    return x.metric > y.metric;
  };

  stack.emplace_back(0, aabb_metric_f(nodes.front().aabb));

  while (stack.size()) {
    auto current = stack.back();
    stack.pop_back();
    if (current.metric > result.metric()) {
      continue;
    }
    const auto &node = nodes[current.id];
    const auto &data = node.get_data();
    if (!node.is_leaf()) {
      auto current_offset = stack.size();
      auto it = nodes.begin() + data[0];
      auto end = it + data[1];
      auto next_id = data[0];
      while (it != end) {
        auto metric = aabb_metric_f(it->aabb);
        if (metric <= result.metric()) {
          stack.emplace_back(next_id, metric);
        }
        ++it;
        ++next_id;
      }
      std::sort(stack.begin() +
                    std::max(Index(current_offset) - data[1], Index(0)),
                stack.end(), compare);
    } else {
      for (const auto &id : tf::make_range(ids.begin() + data[0], data[1])) {
        auto closest_pt = closest_point_f(id);
        result.update(id, closest_pt);
      }
    }
  }
}

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename Result>
auto tree_closest_point_using_heap(
    const buffer<tree_node<Index, RealT, N>> &nodes, const buffer<Index> &ids,
    const F0 &aabb_metric_f, const F1 &closest_point_f, Result &result) {
  if (!nodes.size())
    return;
  using real_type = RealT;
  struct holder_t {
    real_type metric;
    Index id;

    holder_t(Index id, real_type metric) : metric{metric}, id{id} {};
  };

  tf::small_buffer<holder_t, 256> heap;

  auto compare = [](const auto &x, const auto &y) {
    return x.metric > y.metric;
  };

  heap.emplace_back(0, aabb_metric_f(nodes.front().aabb));
  std::push_heap(heap.begin(), heap.end(), compare);

  while (heap.size()) {
    std::pop_heap(heap.begin(), heap.end(), compare);
    auto current = heap.back();
    heap.pop_back();
    if (current.metric > result.metric()) {
      continue;
    }
    const auto &node = nodes[current.id];
    const auto &data = node.get_data();
    if (!node.is_leaf()) {
      auto it = nodes.begin() + data[0];
      auto end = it + data[1];
      auto next_id = data[0];
      while (it != end) {
        auto metric = aabb_metric_f(it->aabb);
        if (metric <= result.metric()) {
          heap.emplace_back(next_id, metric);
          std::push_heap(heap.begin(), heap.end(), compare);
        }
        ++it;
        ++next_id;
      }
    } else {
      for (const auto &id : tf::make_range(ids.begin() + data[0], data[1])) {
        auto closest_pt = closest_point_f(id);
        result.update(id, closest_pt);
      }
    }
  }
}

} // namespace tf::implementation
