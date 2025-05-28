/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./aabb.hpp"
#include <cmath>

namespace tf {
template <typename T, std::size_t N>
auto minimal_maximal_distance2(const aabb<T, N> &a,
                                           const aabb<T, N> &b) -> T {
  return std::max((a.min - b.min).length2(), (a.max - b.max).length2());
}

template <typename T, std::size_t N>
auto minimal_maximal_distance(const aabb<T, N> &a,
                                          const aabb<T, N> &b) -> T {
  return std::sqrt(minimal_maximal_distance2(a, b));
}
} // namespace tf
