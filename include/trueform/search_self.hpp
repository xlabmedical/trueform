/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./implementation/tree_self_search.hpp"
#include "./mod_tree.hpp"
#include "./tree.hpp"
#include <atomic>
namespace tf {
namespace implementation {
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

template <typename Index, typename Tree, typename F0, typename F1>
auto search_self_dispatch(const Tree &tree, const F0 &check_aabbs,
                          const F1 &primitive_apply, int paralelism_depth = 6)
    -> bool {
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
    return implementation::search_self(tree, check_aabbs, apply_f, abort_f,
                                       paralelism_depth);
  } else {
    auto apply_f = [primitive_apply](Index id0, Index id1) -> bool {
      primitive_apply(id0, id1);
      return false;
    };
    auto abort_f = [] { return false; };
    return implementation::search_self(tree, check_aabbs, apply_f, abort_f,
                                       paralelism_depth);
  }
}
} // namespace implementation
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
///
/// @return bool
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto search_self(const tf::tree<Index, RealT, N> &tree, const F0 &check_aabbs,
                 const F1 &primitive_apply, int paralelism_depth = 6) -> bool {
  return implementation::search_self_dispatch<Index>(
      tree, check_aabbs, primitive_apply, paralelism_depth);
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
///
/// @return bool
template <typename Index, typename RealT, std::size_t N, typename F0,
          typename F1>
auto search_self(const tf::mod_tree<Index, RealT, N> &tree,
                 const F0 &check_aabbs, const F1 &primitive_apply,
                 int paralelism_depth = 6) -> bool {
  return implementation::search_self_dispatch<Index>(
      tree, check_aabbs, primitive_apply, paralelism_depth);
}
} // namespace tf
