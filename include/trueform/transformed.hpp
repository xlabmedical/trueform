/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./aabb.hpp"
#include "./ray.hpp"
#include "./transformation.hpp"

namespace tf {

/// @ingroup geometry
/// @brief Compose two affine transformations.
///
/// Given two affine transformations, this returns a new transformation
/// that is equivalent to applying `_this` first, then `transform`.
///
/// This performs standard matrix composition:
/// `result = transform * _this`
///
/// @tparam T Scalar type of the left-hand transformation.
/// @tparam Dims Dimensionality of the transformation.
/// @tparam U Scalar type of the right-hand transformation.
/// @param _this The transformation to apply first.
/// @param transform The transformation to apply second.
/// @return A new @ref tf::transformation representing the composed
/// transformation.
template <typename T, std::size_t Dims, typename U>
auto transformed(const transformation<T, Dims> &_this,
                 const transformation<U, Dims> &transform) {
  using real_t = decltype(_this(0, 0) * transform(0, 0));
  transformation<real_t, Dims> out_array;
  for (std::size_t i = 0; i < Dims; ++i) {
    for (std::size_t j = 0; j < Dims; ++j) {
      out_array(i, j) = 0;
      for (std::size_t k = 0; k < Dims; ++k) {
        out_array(i, j) += transform(i, k) * _this(k, j);
      }
    }
  }
  for (std::size_t i = 0; i < Dims; ++i) {
    out_array(i, Dims) = transform(i, Dims);
    for (std::size_t j = 0; j < Dims; ++j) {
      out_array(i, Dims) += transform(i, j) * _this(j, Dims);
    }
  }
  return out_array;
}

/// @ingroup geometry
/// @brief Apply a transformation to an axis-aligned bounding box (AABB).
///
/// Computes a conservative transformed bounding box by applying the given
/// affine transformation to the input AABB. The resulting box is guaranteed
/// to contain the transformed shape, even under rotation or shear.
///
///
/// @tparam T Scalar type of the input AABB.
/// @tparam Dims Dimensionality of the AABB and transformation.
/// @tparam U Scalar type of the transformation matrix.
/// @param _this The input AABB to be transformed.
/// @param transform The affine transformation to apply.
/// @return A new AABB that conservatively bounds the transformed input.
template <typename T, std::size_t Dims, typename U>
auto transformed(const aabb<T, Dims> &_this,
                 const transformation<U, Dims> &transform) {
  using real_t = decltype(_this.max[0] * transform(0, 0));
  aabb<real_t, Dims> out;
  auto size = Dims;
  for (decltype(size) i = 0; i < size; ++i) {
    out.max[i] = out.min[i] = transform(i, size);
    for (decltype(size) j = 0; j < size; ++j) {
      std::array<decltype(transform(i, j) * _this.min[j]), 2> vals{
          transform(i, j) * _this.min[j], transform(i, j) * _this.max[j]};
      auto mode = vals[0] > vals[1];
      out.min[i] += vals[mode];
      out.max[i] += vals[1 - mode];
    }
  }
  return out;
}

/// @ingroup geometry
/// @brief Apply a transformation to a ray
///
/// @tparam T Scalar type of the input AABB.
/// @tparam Dims Dimensionality of the AABB and transformation.
/// @tparam U Scalar type of the transformation matrix.
/// @param _this The input ray to be transformed.
/// @param transform The affine transformation to apply.
/// @return A new ray in the transformed space
template <typename T, std::size_t Dims, typename U>
auto transformed(const ray<T, Dims> &_this,
                 const transformation<U, Dims> &transform) {
  ray<T, Dims> out{transform.transform_point(_this.origin),
                   transform.transform_vector(_this.direction)};
  return out;
}
} // namespace tf
