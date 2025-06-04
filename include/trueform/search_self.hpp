/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./implementation/tree_self_search.hpp"
#include "./mod_tree.hpp"
#include "./tree.hpp"
namespace tf {
/// @brief Perform a parallel search of a spatial tree against itself.
///
///
/// @param tree The spatial tree.
/// @param check_aabbs Predicate that decides whether to recurse into a pair of
/// nodes.
///                    Signature: `(const tf::aabb<RealT, N>& aabb0, const
///                    tf::aabb<RealT, N>& aabb1) -> bool`
/// @param primitive_apply Function called for each pair of primitive IDs in
/// intersecting leaves.
///                        Signature: `(Index id0, Index id1) -> bool`
///                        **Must be thread-safe** if it accesses shared memory.
/// @param abort Function periodically called to determine if the search should
/// be aborted.
///              Signature: `() -> bool`
///
/// @return bool
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename F2>
auto search_self(const tf::tree<Index, RealT, N> &tree, const F0 &check_aabbs,
                 const F1 &primitive_apply, const F2 &abort,
                 int paralelism_depth = 6) -> bool {
  return tf::implementation::tree_self_search(
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

/// @brief Perform a parallel search of a spatial tree against itself.
///
///
/// @param tree The spatial tree.
/// @param check_aabbs Predicate that decides whether to recurse into a pair of
/// nodes.
///                    Signature: `(const tf::aabb<RealT, N>& aabb0, const
///                    tf::aabb<RealT, N>& aabb1) -> bool`
/// @param primitive_apply Function called for each pair of primitive IDs in
/// intersecting leaves.
///                        Signature: `(Index id0, Index id1) -> bool`
///                        **Must be thread-safe** if it accesses shared memory.
/// @param abort Function periodically called to determine if the search should
/// be aborted.
///              Signature: `() -> bool`
///
/// @return bool
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename F2>
auto search_self(const tf::mod_tree<Index, RealT, N> &tree,
                 const F0 &check_aabbs, const F1 &primitive_apply,
                 const F2 &abort, int paralelism_depth = 6) -> bool {
  if (!search_self(tree.main_tree(), check_aabbs, primitive_apply, abort,
                   paralelism_depth))
    return search_self(tree.delta_tree(), check_aabbs, primitive_apply, abort,
                       paralelism_depth);
  else
    return true;
}
} // namespace tf
