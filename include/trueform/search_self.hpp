/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./implementation/tree_self_search.hpp"
#include "./tree.hpp"
namespace tf {

template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename F2>
auto search_self(const tf::tree<Index, RealT, N> &tree, const F0 &check_aabbs,
                 const F1 &primitive_apply, const F2 &abort,
                 int paralelism_depth = 6) -> void {
  tf::implementation::tree_self_search(
      tree.nodes(), tree.ids(), check_aabbs,
      [primitive_apply](const auto &ids0, const auto &ids1, bool is_self) {
        for (Index i0 = 0; i0 < Index(ids0.size()); ++i0) {
          auto id0 = ids0[i0];
          for (Index i1 = (i0 + 1) * is_self; i1 < Index(ids1.size()); ++i1) {
            auto id1 = ids1[i1];
            if (primitive_apply(id0, id1))
              return true;
          }
        }
        return false;
      },
      abort, paralelism_depth);
}
} // namespace tf
