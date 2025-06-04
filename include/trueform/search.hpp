/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./implementation/tree_dual_search.hpp"
#include "./implementation/tree_search.hpp"
#include "./mod_tree.hpp"
#include "./tree.hpp"
#include <tbb/parallel_invoke.h>
namespace tf {
/// @brief Perform a spatial query against a single tree structure.
///
/// Iterates through the tree and applies a user-provided callback to all
/// primitive IDs whose AABBs intersect the query condition.
///
/// @param tree The spatial tree to search.
/// @param check_aabb A predicate that determines whether a node's AABB should
/// be traversed.
///                   Signature: `bool(const tf::aabb<RealT, N>& aabb)`
/// @param primitive_apply A function applied to each matching primitive ID.
/// Return `true` to abort early.
///                        Signature: `(Index id) -> bool`
///
/// @return bool
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto search(const tf::tree<Index, RealT, N> &tree, const F0 &check_aabb,
            const F1 &primitive_apply) -> bool {
  return tf::implementation::tree_search(tree.nodes(), tree.ids(), check_aabb,
                                         [primitive_apply](const auto &r) {
                                           for (const auto &id : r)
                                             if (primitive_apply(id))
                                               return true;
                                           return false;
                                         });
}

/// @brief Perform a spatial query against a single tree structure.
///
/// Iterates through the tree and applies a user-provided callback to all
/// primitive IDs whose AABBs intersect the query condition.
///
/// @param tree The spatial tree to search.
/// @param check_aabb A predicate that determines whether a node's AABB should
/// be traversed.
///                   Signature: `bool(const tf::aabb<RealT, N>& aabb)`
/// @param primitive_apply A function applied to each matching primitive ID.
/// Return `true` to abort early.
///                        Signature: `(Index id) -> bool`
///
/// @return bool
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto search(const tf::mod_tree<Index, RealT, N> &tree, const F0 &check_aabb,
            const F1 &primitive_apply) -> bool {
  if (!search(tree.main_tree(), check_aabb, primitive_apply))
    return search(tree.delta_tree(), check_aabb, primitive_apply);
  else
    return true;
}

/// @brief Perform a parallel pairwise search between two spatial trees.
///
///
/// @param tree0 The first spatial tree.
/// @param tree1 The second spatial tree.
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
auto search(const tf::tree<Index, RealT, N> &tree0,
            const tf::tree<Index, RealT, N> &tree1, const F0 &check_aabbs,
            const F1 &primitive_apply, const F2 &abort,
            int paralelism_depth = 6) -> bool {
  return tf::implementation::tree_dual_search(
      tree0.nodes(), tree0.ids(), tree1.nodes(), tree1.ids(), check_aabbs,
      [primitive_apply](const auto &r0, const auto &r1) {
        for (const auto &id0 : r0)
          for (const auto &id1 : r1)
            if (primitive_apply(id0, id1))
              return true;
        return false;
      },
      abort, paralelism_depth);
}

/// @brief Perform a parallel pairwise search between two spatial trees.
///
///
/// @param tree0 The first spatial tree.
/// @param tree1 The second spatial tree.
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
auto search(const tf::mod_tree<Index, RealT, N> &tree0,
            const tf::tree<Index, RealT, N> &tree1, const F0 &check_aabbs,
            const F1 &primitive_apply, const F2 &abort,
            int paralelism_depth = 6) -> bool {
  if (!search(tree0.main_tree(), tree1, check_aabbs, primitive_apply, abort,
              paralelism_depth))
    return search(tree0.delta_tree(), tree1, check_aabbs, primitive_apply,
                  abort, paralelism_depth);
  else
    return true;
}

/// @brief Perform a parallel pairwise search between two spatial trees.
///
///
/// @param tree0 The first spatial tree.
/// @param tree1 The second spatial tree.
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
auto search(const tf::tree<Index, RealT, N> &tree0,
            const tf::mod_tree<Index, RealT, N> &tree1, const F0 &check_aabbs,
            const F1 &primitive_apply, const F2 &abort,
            int paralelism_depth = 6) -> bool {
  if (!search(tree0, tree1.main_tree(), check_aabbs, primitive_apply, abort,
              paralelism_depth))
    return search(tree0, tree1.delta_tree(), check_aabbs, primitive_apply,
                  abort, paralelism_depth);
  else
    return true;
}

/// @brief Perform a parallel pairwise search between two spatial trees.
///
///
/// @param tree0 The first spatial tree.
/// @param tree1 The second spatial tree.
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
auto search(const tf::mod_tree<Index, RealT, N> &tree0,
            const tf::mod_tree<Index, RealT, N> &tree1, const F0 &check_aabbs,
            const F1 &primitive_apply, const F2 &abort,
            int paralelism_depth = 6) -> bool {
  if (!search(tree0, tree1.main_tree(), check_aabbs, primitive_apply, abort,
              paralelism_depth))
    return search(tree0, tree1.delta_tree(), check_aabbs, primitive_apply,
                  abort, paralelism_depth);
  else
    return true;
}

} // namespace tf
