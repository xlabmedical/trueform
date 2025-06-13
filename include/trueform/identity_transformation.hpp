/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./point_like.hpp"
#include "./value_type.hpp"
#include "./vector_like.hpp"

namespace tf {

template <typename T> class identity_transformation_t {
  auto operator()(std::size_t i, std::size_t j) const -> T { return i == j; }

  template <std::size_t Dims, typename U>
  auto operator()(const point_like<Dims, U> &point) const {
    return point.template as<tf::value_type<U>, Dims>();
  }

  template <std::size_t Dims, typename U>
  auto operator()(const vector_like<Dims, U> &vector) const {
    return vector.template as<tf::value_type<U>, Dims>();
  }
};

template <typename T>
constexpr identity_transformation_t<T> identity_transformation;
} // namespace tf
