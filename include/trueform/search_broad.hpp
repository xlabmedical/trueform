/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./implementation/tree_dual_search.hpp"
#include "./implementation/tree_search.hpp"
#include "./tree.hpp"
namespace tf {
template <typename Index, typename RealT, std::size_t N, typename F0, typename F1>
auto search_broad(const tf::tree<Index, RealT, N> &tree, const F0 &check_aabb,
            const F1 &leaf_apply) -> void {
  tf::implementation::tree_search(tree.nodes(), tree.ids(), check_aabb,
                                  leaf_apply);
}

template <typename Index, typename RealT, std::size_t N, typename F0, typename F1,
          typename F2>
auto search_broad(const tf::tree<Index, RealT, N> &tree0,
            const tf::tree<Index, RealT, N> &tree1, const F0 &check_aabbs,
            const F1 &leaf_apply, const F2 &abort, int paralelism_depth = 6)
    -> void {
  tf::implementation::tree_dual_search(tree0.nodes(), tree0.ids(),
                                       tree1.nodes(), tree1.ids(), check_aabbs,
                                       leaf_apply, abort, paralelism_depth);
}
} // namespace tf
