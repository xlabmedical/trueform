/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./aabb.hpp"

namespace tf {
template <typename T, std::size_t N>
auto aabb_union_inplace(aabb<T, N> &aabb0, const aabb<T, N> &aabb1)
    -> aabb<T, N> & {
  for (int i = 0; i < int(N); i++) {
    aabb0.min[i] = std::min(aabb0.min[i], aabb1.min[i]);
    aabb0.max[i] = std::max(aabb0.max[i], aabb1.max[i]);
  }
  return aabb0;
}

template <typename T, std::size_t N>
auto aabb_union(const aabb<T, N> &aabb0, const aabb<T, N> &aabb1)
    -> aabb<T, N> {
  aabb<T, N> out = aabb0;
  aabb_union_inplace(out, aabb1);
  return out;
}

template <typename T, std::size_t N>
auto aabb_union_inplace(aabb<T, N> &aabb0, const vector<T, N> &pt)
    -> aabb<T, N> & {
  for (int i = 0; i < int(N); i++) {
    aabb0.min[i] = std::min(aabb0.min[i], pt[i]);
    aabb0.max[i] = std::max(aabb0.max[i], pt[i]);
  }
  return aabb0;
}

template <typename T, std::size_t N>
auto aabb_union(const aabb<T, N> &aabb0, const vector<T, N> &pt) -> aabb<T, N> {
  aabb<T, N> out = aabb0;
  aabb_union_inplace(out, pt);
  return out;
}

} // namespace tf
