/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./aabb_from.hpp"
#include <type_traits>
#include <utility>

namespace tf {

struct tree_node_config {
  int inner_size;
  int leaf_size;
};

template <typename F> struct tree_config : private F {
  tree_config(F f, int inner_size, int leaf_size)
      : F(std::move(f)), node_config{inner_size, leaf_size} {}

  template <typename T> auto make_aabb(T &&t) const -> decltype(auto) {
    return F::operator()(static_cast<T &&>(t));
  }

  tree_node_config node_config;
};

template <typename F>
auto config_tree(int inner_size, int leaf_size, F &&make_aabb_f) {
  return tree_config<std::decay_t<F>>{static_cast<F &&>(make_aabb_f),
                                      inner_size, leaf_size};
}

inline auto config_tree(int inner_size, int leaf_size) {
  auto make_aabb_f = [](const auto &x) {
    using tf::aabb_from;
    return aabb_from(x);
  };
  return tree_config<decltype(make_aabb_f)>{std::move(make_aabb_f), inner_size,
                                            leaf_size};
}

} // namespace tf
