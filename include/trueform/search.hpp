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
#include <atomic>

namespace tf {
namespace implementation {
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1, typename F2>
auto search(const tf::tree<Index, RealT, N> &tree0,
            const tf::tree<Index, RealT, N> &tree1, const F0 &check_aabbs,
            const F1 &primitive_apply, const F2 &abort,
            int paralelism_depth = 6) -> bool {
  return tf::implementation::tree_dual_search(
      tree0.nodes(), tree0.ids(), tree1.nodes(), tree1.ids(), check_aabbs,
      [primitive_apply, &tree0, &tree1, &check_aabbs](const auto &r0,
                                                      const auto &r1) {
        for (const auto &id0 : r0)
          for (const auto &id1 : r1)
            if (check_aabbs(tree0.primitive_aabbs()[id0],
                            tree1.primitive_aabbs()[id1]) &&
                primitive_apply(id0, id1))
              return true;
        return false;
      },
      abort, paralelism_depth);
}

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

template <typename Index, typename Tree0, typename Tree1, typename F0,
          typename F1>
auto dual_search_dispatch(const Tree0 &tree0, const Tree1 &tree1,
                          const F0 &check_aabbs, const F1 &primitive_apply,
                          int paralelism_depth = 6) -> bool {

  if constexpr (!std::is_same_v<decltype(primitive_apply(Index(0), Index(0))),
                                void>) {
    std::atomic_bool flag{false};
    auto abort_f = [&flag] { return flag.load(); };
    auto apply_f = [&flag, primitive_apply](Index id0, Index id1) -> bool {
      if (primitive_apply(id0, id1)) {
        flag.store(true);
        return true;
      }
      return false;
    };
    return implementation::search(tree0, tree1, check_aabbs, apply_f, abort_f,
                                  paralelism_depth);
  } else {
    auto apply_f = [primitive_apply](Index id0, Index id1) -> bool {
      primitive_apply(id0, id1);
      return false;
    };
    auto abort_f = [] { return false; };
    return implementation::search(tree0, tree1, check_aabbs, apply_f, abort_f,
                                  paralelism_depth);
  }
}

} // namespace implementation

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
  return tf::implementation::tree_search(
      tree.nodes(), tree.ids(), check_aabb,
      [primitive_apply, &tree, &check_aabb](const auto &r) {
        for (const auto &id : r)
          if (check_aabb(tree.primitive_aabbs()[id])) {
            if constexpr (std::is_same_v<decltype(primitive_apply(id)), void>) {
              primitive_apply(id);
            } else {
              if (primitive_apply(id))
                return true;
            }
          }
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
///
/// @return bool
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto search(const tf::tree<Index, RealT, N> &tree0,
            const tf::tree<Index, RealT, N> &tree1, const F0 &check_aabbs,
            const F1 &primitive_apply, int paralelism_depth = 6) -> bool {
  return implementation::dual_search_dispatch<Index>(
      tree0, tree1, check_aabbs, primitive_apply, paralelism_depth);
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
///
/// @return bool
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto search(const tf::mod_tree<Index, RealT, N> &tree0,
            const tf::tree<Index, RealT, N> &tree1, const F0 &check_aabbs,
            const F1 &primitive_apply, int paralelism_depth = 6) -> bool {
  return implementation::dual_search_dispatch<Index>(
      tree0, tree1, check_aabbs, primitive_apply, paralelism_depth);
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
///
/// @return bool
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto search(const tf::tree<Index, RealT, N> &tree0,
            const tf::mod_tree<Index, RealT, N> &tree1, const F0 &check_aabbs,
            const F1 &primitive_apply, int paralelism_depth = 6) -> bool {
  return implementation::dual_search_dispatch<Index>(
      tree0, tree1, check_aabbs, primitive_apply, paralelism_depth);
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
///
/// @return bool
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto search(const tf::mod_tree<Index, RealT, N> &tree0,
            const tf::mod_tree<Index, RealT, N> &tree1, const F0 &check_aabbs,
            const F1 &primitive_apply, int paralelism_depth = 6) -> bool {
  return implementation::dual_search_dispatch<Index>(
      tree0, tree1, check_aabbs, primitive_apply, paralelism_depth);
}

} // namespace tf
