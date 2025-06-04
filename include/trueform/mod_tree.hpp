/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./buffer.hpp"
#include "./indirect_range.hpp"
#include "./mapping.hpp"
#include "./parallel_copy.hpp"
#include "./tree.hpp"
#include <algorithm>

namespace tf {
template <typename Index, typename RealType, std::size_t Dims> class mod_tree {
public:
  template <typename Partitioner, typename Range, typename FC>
  auto build(const Range &objects, const tf::tree_config<FC> &config) -> void {
    _delta_ids.clear();
    _delta_tree.clear();
    _main_tree.build(objects, make_aabb, config);
  }

  template <typename Range, typename FC>
  auto build(const Range &objects, const tf::tree_config<FC> &config) -> void {
    return build<strategy::nth_element_t>(objects, config);
  }

  template <typename Range0, typename Range1, typename F, typename FC>
  auto update(const Range0 &objects, const Range1 &ids, const F &keep_if,
              const tf::tree_config<FC> &config) {
    update_main_tree(keep_if);
    update_delta_tree(objects, ids, keep_if, config);
  }

  template <typename Range, typename Range1, typename Range2, typename F,
            typename FC>
  auto update_tree(const Range &objects,
                   const tf::mapping<Range1, Range2> &mapping, const F &keep_if,
                   const tf::tree_config<FC> &config) {
    update_main_tree(mapping.f(), keep_if);
    update_delta_tree(objects, mapping, keep_if, config);
  }

  auto main_tree() const -> const tree<Index, RealType, Dims> & {
    return _main_tree;
  }

  auto main_tree() -> tree<Index, RealType, Dims> & { return _main_tree; }

  auto delta_tree() const -> const tree<Index, RealType, Dims> & {
    return _delta_tree;
  }

  auto delta_tree() -> tree<Index, RealType, Dims> & { return _delta_tree; }

  auto clear() {
    _main_tree.clear();
    _delta_tree.clear();
    _delta_ids.clear();
  }

private:
  template <typename Range, typename F>
  auto update_main_tree(const Range &id_map, const F &keep_if) {
    auto &ids = _main_tree.ids();
    tf::parallel_apply(_main_tree.nodes(), [&](auto &node) {
      if (!node.is_leaf())
        return;
      auto &data = node.get_data();
      auto c_ids = tf::make_range(ids.begin() + data[0], data[1]);
      // we apply the mapping
      for (auto &x : c_ids)
        x = id_map[x];
      // we move invalidated ids to the end of the subrange
      // and set new size
      node.set_data(data[0],
                    std::partition(c_ids.begin(), c_ids.end(), keep_if) -
                        c_ids.begin());
    });
  }
  template <typename F> auto update_main_tree(const F &keep_if) {
    auto &ids = _main_tree.ids();
    tf::parallel_apply(_main_tree.nodes(), [&](auto &node) {
      if (!node.is_leaf())
        return;
      auto &data = node.get_data();
      auto c_ids = tf::make_range(ids.begin() + data[0], data[1]);
      // we move invalidated ids to the end of the subrange
      // and set new size
      node.set_data(data[0],
                    std::partition(c_ids.begin(), c_ids.end(), keep_if) -
                        c_ids.begin());
    });
  }

  template <typename Range0, typename Range1, typename F, typename FC>
  auto update_delta_tree(const Range0 &objects, const Range1 &ids,
                         const F &keep_if, const tf::tree_config<FC> &config) {
    auto n_additional_objects = ids.size();
    _delta_ids.allocate(n_additional_objects + _delta_ids.size());
    // keep all old ids that are not in the
    // other region (as those will get copied from there)
    auto write_to =
        std::copy_if(_delta_tree.ids().begin(), _delta_tree.ids().end(),
                     _delta_ids.begin(), keep_if);
    write_to = std::copy(ids.begin(), ids.end(), write_to);
    // this will only bump down the end pointer
    _delta_ids.allocate(write_to - _delta_ids.begin());
    _delta_tree.build(tf::make_indirect_range(_delta_ids, objects), config);
    // ids in the tree are a permutation of
    // [0, ..., _small_ids.size()]
    // so we remap them to the real ids
    tf::parallel_copy(tf::make_indirect_range(_delta_tree.ids(), _delta_ids),
                      _delta_tree.ids());
  }

  template <typename Range, typename Range1, typename Range2, typename F,
            typename FC>
  auto update_delta_tree(const Range &objects,
                         const tf::mapping<Range1, Range2> &mapping,
                         const F &keep_if, const tf::tree_config<FC> &config) {
    auto n_additional_objects = mapping.kept_ids().size();
    _delta_ids.allocate(n_additional_objects + _delta_ids.size());
    // keep all old ids that are not in the
    // other region (as those will get copied from there)
    auto mapped_ids = tf::make_indirect_range(_delta_tree.ids(), mapping.f());
    auto write_to = std::copy_if(mapped_ids.begin(), mapped_ids.end(),
                                 _delta_ids.begin(), keep_if);
    write_to = std::copy(mapping.kept_ids().begin(), mapping.kept_ids().end(),
                         write_to);
    // this will only bump down the end pointer
    _delta_ids.allocate(write_to - _delta_ids.begin());
    _delta_tree.build(tf::make_indirect_range(_delta_ids, objects), config);
    // ids in the tree are a permutation of
    // [0, ..., _small_ids.size()]
    // so we remap them to the real ids
    tf::parallel_copy(tf::make_indirect_range(_delta_tree.ids(), _delta_ids),
                      _delta_tree.ids());
  }

  tree<Index, RealType, Dims> _main_tree;
  tree<Index, RealType, Dims> _delta_tree;
  tf::buffer<Index> _delta_ids;
};
} // namespace tf
