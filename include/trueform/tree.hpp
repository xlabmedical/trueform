/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./implementation/build_tree_nodes.hpp"
#include "./partitioning.hpp"
#include "./tree_config.hpp"

namespace tf {
template <typename Index, typename RealT, std::size_t N> class tree {
public:
  template <typename Partitioner, typename Range, typename FC>
  auto build(const Range &objects, const tf::tree_config<FC> &config) -> void {
    _aabbs.allocate(objects.size());
    tf::parallel_apply(std::forward_as_tuple(objects, _aabbs),
                       [&](const auto &object, auto &aabb) {
                         aabb = config.make_aabb(object);
                       });
    tf::implementation::build_tree_nodes<Partitioner>(_nodes, _ids, _aabbs,
                                                      config.node_config);
  }

  template <typename Range, typename FC>
  auto build(const Range &objects, const tf::tree_config<FC> &config) -> void {
    return build<strategy::nth_element_t>(objects, config);
  }

  template <typename Range, typename FC>
  auto build(strategy::nth_element_t, const Range &objects,
             const tf::tree_config<FC> &config) -> void {
    return build<strategy::nth_element_t>(objects, config);
  }

  template <typename Range, typename FC>
  auto build(strategy::pdq_t, const Range &objects,
             const tf::tree_config<FC> &config) -> void {
    return build<strategy::pdq_t>(objects, config);
  }

  template <typename Range, typename FC>
  auto build(strategy::floyd_rivest_t, const Range &objects,
             const tf::tree_config<FC> &config) -> void {
    return build<strategy::floyd_rivest_t>(objects, config);
  }

  template <typename Range, typename FC>
  auto build(strategy::median_of_medians_t, const Range &objects,
             const tf::tree_config<FC> &config) -> void {
    return build<strategy::median_of_medians_t>(objects, config);
  }

  template <typename Range, typename FC>
  auto build(strategy::median_of_ninthers_t, const Range &objects,
             const tf::tree_config<FC> &config) -> void {
    return build<strategy::median_of_ninthers_t>(objects, config);
  }
  template <typename Range, typename FC>
  auto build(strategy::median_of_3_random_t, const Range &objects,
             const tf::tree_config<FC> &config) -> void {
    return build<strategy::median_of_3_random_t>(objects, config);
  }

  template <typename Range, typename FC>
  auto build(strategy::heap_select_t, const Range &objects,
             const tf::tree_config<FC> &config) -> void {
    return build<strategy::heap_select_t>(objects, config);
  }

  auto primitive_aabbs() const -> const tf::buffer<tf::aabb<RealT, N>> & {
    return _aabbs;
  }

  auto nodes() const -> const tf::buffer<tf::tree_node<Index, RealT, N>> & {
    return _nodes;
  }

  auto ids() const -> const tf::buffer<Index> & { return _ids; }

  auto clear() {
    _aabbs.clear();
    _nodes.clear();
    _ids.clear();
  }

private:
  tf::buffer<tf::aabb<RealT, N>> _aabbs;
  tf::buffer<tf::tree_node<Index, RealT, N>> _nodes;
  tf::buffer<Index> _ids;
};
} // namespace tf
