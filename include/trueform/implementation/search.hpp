/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "../form.hpp"
#include "../inject_id.hpp"
#include "../mod_form.hpp"
#include "../mod_tree.hpp"
#include "../transformed.hpp"
#include "../tree.hpp"
#include "./tree_dual_search.hpp"
#include "./tree_search.hpp"
#include <atomic>

namespace tf::implementation {
template <typename Index, typename RealT, std::size_t N, typename Policy,
          typename F0, typename F1>
auto search(const tf::form<Index, RealT, N, Policy> &form, const F0 &check_aabb,
            const F1 &primitive_apply) -> bool {
  return tf::implementation::tree_search(
      form.tree().nodes(), form.tree().ids(),
      [&check_aabb, &form](const auto &aabb) {
        return check_aabb(tf::transformed(aabb, form.frame().transformation()));
      },
      [primitive_apply, &form, &check_aabb](const auto &r) {
        for (const auto &id : r)
          if (check_aabb(tf::transformed(form.tree().primitive_aabbs()[id],
                                         form.frame().transformation()))) {
            if constexpr (std::is_same_v<decltype(primitive_apply(id)), void>) {
              primitive_apply(tf::inject_id(id, tf::transformed(form[id])));
            } else {
              if (primitive_apply(tf::inject_id(id, tf::transformed(form[id]))))
                return true;
            }
          }
        return false;
      });
}

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

template <typename Index, typename RealT, std::size_t N, typename Policy0,
          typename Policy1, typename F0, typename F1, typename F2>
auto search(const tf::form<Index, RealT, N, Policy0> &form0,
            const tf::form<Index, RealT, N, Policy1> &form1,
            const F0 &check_aabbs, const F1 &primitive_apply, const F2 &abort,
            int paralelism_depth = 6) -> bool {
  auto aabb_f = [&](const auto &aabb0, const auto &aabb1) -> bool {
    return check_aabbs(tf::transformed(aabb0, form0.transformation()),
                       tf::transformed(aabb1, form1.transformation()));
  };
  return tf::implementation::tree_dual_search(
      form0.tree().nodes(), form0.tree().ids(), form1.tree().nodes(),
      form1.tree().ids(), aabb_f,
      [primitive_apply, &form0, &form1, &aabb_f](const auto &r0,
                                                 const auto &r1) {
        for (const auto &id0 : r0) {
          auto obj0 = tf::inject_id(
              id0, tf::transformed(form0[id0], form0.transformation()));
          for (const auto &id1 : r1)
            if (aabb_f(form0.tree().primitive_aabbs()[id0],
                       form1.tree().primitive_aabbs()[id1]) &&
                primitive_apply(
                    obj0, tf::inject_id(
                              id1, tf::transformed(form1[id1],
                                                   form1.transformation()))))
              return true;
        }
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

template <typename Index, typename RealT, std::size_t N, typename Policy0,
          typename Policy1, typename F0, typename F1, typename F2>
auto search(const tf::mod_form<Index, RealT, N, Policy0> &form0,
            const tf::form<Index, RealT, N, Policy1> &form1,
            const F0 &check_aabbs, const F1 &primitive_apply, const F2 &abort,
            int paralelism_depth = 6) -> bool {
  if (!search(form0.main_form(), form1, check_aabbs, primitive_apply, abort,
              paralelism_depth))
    return search(form0.delta_form(), form1, check_aabbs, primitive_apply,
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

template <typename Index, typename RealT, std::size_t N, typename Policy0,
          typename Policy1, typename F0, typename F1, typename F2>
auto search(const tf::form<Index, RealT, N, Policy0> &form0,
            const tf::mod_form<Index, RealT, N, Policy1> &form1,
            const F0 &check_aabbs, const F1 &primitive_apply, const F2 &abort,
            int paralelism_depth = 6) -> bool {
  if (!search(form0, form1.main_form(), check_aabbs, primitive_apply, abort,
              paralelism_depth))
    return search(form0, form1.delta_form(), check_aabbs, primitive_apply,
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

template <typename Index, typename RealT, std::size_t N, typename Policy0,
          typename Policy1, typename F0, typename F1, typename F2>
auto search(const tf::mod_form<Index, RealT, N, Policy0> &form0,
            const tf::mod_form<Index, RealT, N, Policy1> &form1,
            const F0 &check_aabbs, const F1 &primitive_apply, const F2 &abort,
            int paralelism_depth = 6) -> bool {
  if (!search(form0, form1.main_form(), check_aabbs, primitive_apply, abort,
              paralelism_depth))
    return search(form0, form1.delta_form(), check_aabbs, primitive_apply,
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
// this is "duplicated", because in the form case we additionally
// optimize in the leaves, where we only transform once per
// outer loop, instead of always in the applier
template <typename Index, typename Form0, typename Form1, typename F0,
          typename F1>
auto dual_form_search_dispatch(const Form0 &form0, const Form1 &form1,
                               const F0 &check_aabbs, const F1 &primitive_apply,
                               int paralelism_depth = 6) -> bool {

  if constexpr (!std::is_same_v<
                    decltype(primitive_apply(
                        tf::inject_id(Index(0),
                                      tf::transformed(form0[Index(0)],
                                                      form0.transformation())),
                        tf::inject_id(Index(0), tf::transformed(
                                                    form1[Index(0)],
                                                    form1.transformation())))),
                    void>) {
    std::atomic_bool flag{false};
    auto abort_f = [&flag] { return flag.load(); };
    auto apply_f = [&flag, primitive_apply](auto &&obj0, auto &&obj1) -> bool {
      if (primitive_apply(obj0, obj1)) {
        flag.store(true);
        return true;
      }
      return false;
    };
    return implementation::search(form0, form1, check_aabbs, apply_f, abort_f,
                                  paralelism_depth);
  } else {
    auto apply_f = [primitive_apply](auto &&obj0, auto &&obj1) -> bool {
      primitive_apply(obj0, obj1);
      return false;
    };
    auto abort_f = [] { return false; };
    return implementation::search(form0, form1, check_aabbs, apply_f, abort_f,
                                  paralelism_depth);
  }
}

} // namespace tf::implementation
