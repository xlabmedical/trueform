/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./value_type.hpp"
#include "./vector.hpp"
#include "./vector_like.hpp"

namespace tf {
/// @ingroup geometry
/// @brief Computes the cross product of two 3D vectors.
///
/// The cross product is defined only in 3D space. It produces a new vector
/// that is orthogonal to both input vectors and whose direction follows the
/// right-hand rule.
///
/// @tparam T0 The type of the first vector-like operand.
/// @tparam T1 The type of the second vector-like operand.
/// @param pt0 The first 3D vector-like operand.
/// @param pt1 The second 3D vector-like operand.
/// @return A `vector<common_value<T0, T1>, 3>` representing the cross product.
template <typename T0, typename T1>
auto cross(const vector_like<3, T0> &pt0, const vector_like<3, T1> &pt1) {
  return vector<tf::common_value<T0, T1>, 3>{{
      pt0[1] * pt1[2] - pt0[2] * pt1[1], //
      pt0[2] * pt1[0] - pt0[0] * pt1[2], //
      pt0[0] * pt1[1] - pt0[1] * pt1[0]  //
  }};
}
} // namespace tf
