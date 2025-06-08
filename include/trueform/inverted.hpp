/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./implementation/invert_matrix_2x2.hpp"
#include "./implementation/invert_matrix_3x3.hpp"
#include "./transformation.hpp"

namespace tf {
/// @ingroup geometry
/// @brief Computes the inverse of @tf::transformation
template <typename RealT, std::size_t Dims>
auto inverted(const transformation<RealT, Dims> &transform) {
  static_assert(Dims == 2 || Dims == 3);
  tf::transformation<RealT, Dims> out;
  if constexpr (Dims == 3)
    implementation::invert_matrix_3x3(transform, out);
  else
    implementation::invert_matrix_2x2(transform, out);
  for (std::size_t i = 0; i < Dims; ++i) {
    out(i, Dims) = 0;
    for (std::size_t j = 0; j < Dims; ++j) {
      out(i, Dims) -= out(i, j) * transform(j, Dims);
    }
  }
  return out;
}
} // namespace tf
