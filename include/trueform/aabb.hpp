/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./vector.hpp"

namespace tf {

template <typename T, std::size_t N> class aabb {
public:
  vector<T, N> min;
  vector<T, N> max;

  // Default constructor
  aabb() = default;

  // Constructor from min and max
  aabb(const vector<T, N> &min_, const vector<T, N> &max_)
      : min(min_), max(max_) {}

  // Return center point of the AABB
  auto center() const -> vector<T, N> {
    return (min + max) * static_cast<T>(0.5);
  }

  // Return diagonal vector (max - min)
  auto diagonal() const -> vector<T, N> { return max - min; }

  // Return dimensionality (compile-time constant)
  constexpr auto size() -> std::size_t { return N; }
};

template <typename T, std::size_t N>
auto make_aabb(const vector<T, N> &min, const vector<T, N> &max) -> aabb<T, N> {
  return aabb<T, N>(min, max);
}

} // namespace tf
