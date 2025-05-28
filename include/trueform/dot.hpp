/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./vector.hpp"

namespace tf {

template <typename T, std::size_t N>
auto dot(const vector<T, N> &a, const vector<T, N> &b) -> T {
  T sum = T{};
  for (std::size_t i = 0; i < N; ++i) {
    sum += a[i] * b[i];
  }
  return sum;
}
} // namespace tf
