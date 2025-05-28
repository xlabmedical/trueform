/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./aabb.hpp"
#include "./vector.hpp"

namespace tf {
template <typename T, std::size_t N>
auto aabb_from(const vector<T, N> &pt) -> aabb<T, N> {
  return aabb<T, N>{pt, pt};
}
} // namespace tf
