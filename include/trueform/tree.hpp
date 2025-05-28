/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./aabb_from.hpp"
#include "./implementation/build_tree_nodes.hpp"
#include "./partitioning.hpp"

namespace tf {
template <typename Index, typename RealT, std::size_t N> class tree {
public:
  template <typename Partitioner, typename Range>
  auto build(const Range &objects, const tf::tree_config &config) -> void {
    _aabbs.allocate(objects.size());
    tf::parallel_apply(std::forward_as_tuple(objects, _aabbs),
                       [&](const auto &object, auto &aabb) {
                         using tf::aabb_from;
                         aabb = aabb_from(object);
                       });
    tf::implementation::build_tree_nodes<Partitioner>(_nodes, _ids, _aabbs,
                                                      config);
  }

  template <typename Partitioner, typename Range, typename F>
  auto build(const Range &objects, const F &make_aabb,
             const tf::tree_config &config) -> void {
    _aabbs.allocate(objects.size());
    tf::parallel_apply(
        std::forward_as_tuple(objects, _aabbs),
        [&](const auto &object, auto &aabb) { aabb = make_aabb(object); });
    tf::implementation::build_tree_nodes<Partitioner>(_nodes, _ids, _aabbs,
                                                      config);
  }

  template <typename Range>
  auto build(const Range &objects, const tf::tree_config &config) -> void {
    return build<strategy::nth_element_t>(objects, config);
  }

  template <typename Range, typename F>
  auto build(const Range &objects, const F &make_aabb,
             const tf::tree_config &config) -> void {
    return build<strategy::nth_element_t>(objects, make_aabb, config);
  }

  template <typename Range>
  auto build(strategy::nth_element_t, const Range &objects,
             const tf::tree_config &config) -> void {
    return build<strategy::nth_element_t>(objects, config);
  }

  template <typename Range, typename F>
  auto build(strategy::nth_element_t, const Range &objects, const F &make_aabb,
             const tf::tree_config &config) -> void {
    return build<strategy::nth_element_t>(objects, make_aabb, config);
  }

  template <typename Range>
  auto build(strategy::pdq_t, const Range &objects,
             const tf::tree_config &config) -> void {
    return build<strategy::pdq_t>(objects, config);
  }

  template <typename Range, typename F>
  auto build(strategy::pdq_t, const Range &objects, const F &make_aabb,
             const tf::tree_config &config) -> void {
    return build<strategy::pdq_t>(objects, make_aabb, config);
  }

  template <typename Range>
  auto build(strategy::floyd_rivest_t, const Range &objects,
             const tf::tree_config &config) -> void {
    return build<strategy::floyd_rivest_t>(objects, config);
  }
  template <typename Range, typename F>
  auto build(strategy::floyd_rivest_t, const Range &objects, const F &make_aabb,
             const tf::tree_config &config) -> void {
    return build<strategy::floyd_rivest_t>(objects, make_aabb, config);
  }

  template <typename Range>
  auto build(strategy::median_of_medians_t, const Range &objects,
             const tf::tree_config &config) -> void {
    return build<strategy::median_of_medians_t>(objects, config);
  }
  template <typename Range, typename F>
  auto build(strategy::median_of_medians_t, const Range &objects,
             const F &make_aabb, const tf::tree_config &config) -> void {
    return build<strategy::median_of_medians_t>(objects, make_aabb, config);
  }

  template <typename Range>
  auto build(strategy::median_of_ninthers_t, const Range &objects,
             const tf::tree_config &config) -> void {
    return build<strategy::median_of_ninthers_t>(objects, config);
  }
  template <typename Range, typename F>
  auto build(strategy::median_of_ninthers_t, const Range &objects,
             const F &make_aabb, const tf::tree_config &config) -> void {
    return build<strategy::median_of_ninthers_t>(objects, make_aabb, config);
  }

  template <typename Range>
  auto build(strategy::median_of_3_random_t, const Range &objects,
             const tf::tree_config &config) -> void {
    return build<strategy::median_of_3_random_t>(objects, config);
  }

  template <typename Range, typename F>
  auto build(strategy::median_of_3_random_t, const Range &objects,
             const F &make_aabb, const tf::tree_config &config) -> void {
    return build<strategy::median_of_3_random_t>(objects, make_aabb, config);
  }

  template <typename Range>
  auto build(strategy::heap_select_t, const Range &objects,
             const tf::tree_config &config) -> void {
    return build<strategy::heap_select_t>(objects, config);
  }

  template <typename Range, typename F>
  auto build(strategy::heap_select_t, const Range &objects, const F &make_aabb,
             const tf::tree_config &config) -> void {
    return build<strategy::heap_select_t>(objects, make_aabb, config);
  }

  auto primitive_aabbs() const -> const tf::buffer<tf::aabb<RealT, N>> & {
    return _aabbs;
  }

  auto nodes() const -> const tf::buffer<tf::tree_node<Index, RealT, N>> & {
    return _nodes;
  }

  auto ids() const -> const tf::buffer<Index> & { return _ids; }

private:
  tf::buffer<tf::aabb<RealT, N>> _aabbs;
  tf::buffer<tf::tree_node<Index, RealT, N>> _nodes;
  tf::buffer<Index> _ids;
};
} // namespace tf
