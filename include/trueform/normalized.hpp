/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./normalize.hpp"

namespace tf {
template <typename T, std::size_t Dims>
auto normalized(const tf::vector<T, Dims> &v) -> tf::vector<T, Dims> {
  auto out = v;
  return normalize(out);
}

template <typename T, std::size_t Dims>
auto normalized(const tf::vector_view<T, Dims> &v) -> tf::vector_view<T, Dims> {
  auto out = v;
  return normalize(out);
}
} // namespace tf
