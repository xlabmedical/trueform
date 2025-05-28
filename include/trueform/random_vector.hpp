/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./random.hpp"
#include "./vector.hpp"

namespace tf {
template <int N, typename T>
auto random_vector(T from, T to) -> tf::vector<T, N> {
  return tf::vector<T, N>{
      {tf::random(from, to), tf::random(from, to), tf::random(from, to)}};
}

template <typename T, std::size_t N> auto random_vector() -> tf::vector<T, N> {
  return tf::random_vector<N>(T(0), T(1));
}
} // namespace tf
