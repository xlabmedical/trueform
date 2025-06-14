/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./point.hpp"
#include "./value_type.hpp"

namespace tf {
template <typename T, std::size_t Dims> struct sphere {
  tf::point<T, Dims> origin;
  T r;
};

template <typename T0, std::size_t Dims, typename T1>
auto make_sphere(const tf::point_like<Dims, T0> &origin, const T1 &r) {
  return sphere<tf::common_value<T0, T1>, Dims>{origin, r};
}
} // namespace tf
