/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./aabb.hpp"
#include "./transformation.hpp"

namespace tf {
template <typename T, std::size_t Dims, typename U>
auto transformed(const transformation<T, Dims> &_this,
                 const transformation<U, Dims> &transform) {
  using real_t = decltype(_this(0, 0) * transform(0, 0));
  transformation<real_t, Dims> out_array;
  for (int i = 0; i < Dims; ++i) {
    for (int j = 0; j < Dims; ++j) {
      out_array(i, j) = 0;
      for (int k = 0; k < Dims; ++k) {
        out_array(i, j) += transform(i, k) * _this(k, j);
      }
    }
  }
  for (int i = 0; i < Dims; ++i) {
    out_array(i, Dims) = transform(i, Dims);
    for (int j = 0; j < Dims; ++j) {
      out_array(i, Dims) += transform(i, j) * _this(j, Dims);
    }
  }
  return out_array;
}

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
} // namespace tf
