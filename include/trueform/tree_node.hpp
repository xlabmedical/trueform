/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./aabb.hpp"
#include <cstdint>

namespace tf {
template <typename Index, typename RealType, std::size_t N> struct tree_node {
  static constexpr Index empty_tag = -2;
  static constexpr Index leaf_tag = -1;

  tf::aabb<RealType, N> aabb;
  std::array<Index, 2> _data;
  std::int8_t axis;
  auto set_axis(std::int8_t _axis) { axis = _axis; }
  auto set_as_leaf() { axis = leaf_tag; }
  auto set_as_empty() { axis = empty_tag; }
  auto is_leaf() const { return axis == leaf_tag; }
  auto is_empty() const { return axis == empty_tag; }
  auto set_data(Index first, Index second) {
    _data[0] = first, _data[1] = second;
  }
  auto get_data() const -> const std::array<Index, 2> { return _data; }
};
} // namespace tf
