/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./vector.hpp"
#include <array>

namespace tf {
template <typename T, std::size_t Dims> class transformation {
public:
  transformation() = default;
  transformation(const std::array<std::array<T, 4>, Dims> &t) : _transform{t} {}
  auto operator()(std::size_t i, std::size_t j) const -> const T & {
    return _transform[i][j];
  }

  auto operator()(std::size_t i, std::size_t j) -> T & {
    return _transform[i][j];
  }

  template <typename Point0, typename Point1>
  auto transform_point(const Point0 &point, Point1 &&out) const {
    auto size = Dims;
    for (decltype(size) i = 0; i < size; ++i) {
      out[i] = _transform[i][size];
      for (decltype(size) j = 0; j < size; ++j) {
        out[i] += point[j] * _transform[i][j];
      }
    }
  }

  template <typename Point> auto transform_point(const Point &point) const {
    tf::vector<decltype(point[0] * T()), Dims> out;
    transform_point(point, out);
    return out;
  }

  template <typename Point0, typename Point1>
  auto transform_vector(const Point0 &point, Point1 &&out) const {
    auto size = Dims;
    for (decltype(size) i = 0; i < size; ++i) {
      out[i] = 0;
      for (decltype(size) j = 0; j < size; ++j) {
        out[i] += point[j] * _transform[i][j];
      }
    }
  }

  template <typename Point> auto transform_vector(const Point &point) const {
    decltype(point - point) out;
    transform_vector(point, out);
    return out;
  }

private:
  std::array<std::array<T, Dims + 1>, Dims> _transform;
};

template <typename T, std::size_t Dims> auto make_identity_transformation() {
  transformation<T, Dims> out;
  for (std::size_t i = 0; i < Dims; ++i)
    for (std::size_t j = 0; j < Dims + 1; ++j)
      out(i, j) = i == j;
  return out;
}

template <typename T, std::size_t Dims>
auto make_transformation_from_translation(
    const tf::vector<T, Dims> &translation) {
  auto out = make_identity_transformation<T, Dims>();
  for (std::size_t i = 0; i < Dims; ++i)
    out(i, Dims) = translation[i];
  return out;
}
} // namespace tf
