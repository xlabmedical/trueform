/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include <array>
#include <cmath>

namespace tf {

template <typename T, std::size_t N> class vector {
public:
  vector() = default;

  vector(std::array<T, N> init) : data{init} {};

  vector(const T *ptr) : data{} {
    for (std::size_t i = 0; i < N; ++i)
      data[i] = ptr[i];
  }

  template <typename U, typename = typename std::enable_if_t<
                            std::is_convertible<T, U>::value, void>>
  operator vector<U, N>() const {
    vector<U, N> out;
    for (std::size_t i = 0; i < N; ++i)
      out[i] = data[i];
    return out;
  }

  template <typename U, typename = typename std::enable_if_t<
                            std::is_convertible<T, U>::value, void>>
  auto as() const -> vector<U, N> {
    return *this;
  }

  // Element access
  auto operator[](std::size_t i) -> T & { return data[i]; }
  auto operator[](std::size_t i) const -> const T & { return data[i]; }

  auto begin() const -> const T * { return data.begin(); }

  auto end() const -> const T * { return data.end(); }

  auto begin() -> T * { return data.begin(); }

  auto end() -> T * { return data.end(); }

  auto length2() const -> T {
    T l2 = 0;
    for (int i = 0; i < int(N); ++i)
      l2 += data[i] * data[i];
    return l2;
  }

  auto length() const -> T { return std::sqrt(length2()); }

  // Compound assignment: +=
  auto operator+=(const vector &other) -> vector & {
    for (std::size_t i = 0; i < N; ++i)
      data[i] += other[i];
    return *this;
  }

  // Compound assignment: -=
  auto operator-=(const vector &other) -> vector & {
    for (std::size_t i = 0; i < N; ++i)
      data[i] -= other[i];
    return *this;
  }

  // Compound assignment: *= scalar
  auto operator*=(T scalar) -> vector & {
    for (std::size_t i = 0; i < N; ++i)
      data[i] *= scalar;
    return *this;
  }

  // Compound assignment: /= scalar
  auto operator/=(T scalar) -> vector & {
    for (std::size_t i = 0; i < N; ++i)
      data[i] /= scalar;
    return *this;
  }

  // Friends using compound forms

  friend auto operator+(vector lhs, const vector &rhs) -> vector {
    return lhs += rhs;
  }

  friend auto operator-(vector lhs, const vector &rhs) -> vector {
    return lhs -= rhs;
  }

  friend auto operator*(vector lhs, T scalar) -> vector {
    return lhs *= scalar;
  }

  friend auto operator*(T scalar, vector rhs) -> vector {
    return rhs *= scalar;
  }

  friend auto operator/(vector lhs, T scalar) -> vector {
    return lhs /= scalar;
  }

  // Unary minus
  friend auto operator-(const vector &v) -> vector {
    vector result;
    for (std::size_t i = 0; i < N; ++i)
      result[i] = -v[i];
    return result;
  }

  // Equality
  friend auto operator==(const vector &a, const vector &b) -> bool {
    return a.data == b.data;
  }

  friend auto operator!=(const vector &a, const vector &b) -> bool {
    return !(a == b);
  }

  friend auto operator<(const vector &a, const vector &b) -> bool {
    return a.data < b.data;
  }

  friend auto operator>(const vector &a, const vector &b) -> bool {
    return a.data > b.data;
  }

  friend auto operator<=(const vector &a, const vector &b) -> bool {
    return !(a > b);
  }

  friend auto operator>=(const vector &a, const vector &b) -> bool {
    return !(a < b);
  }

private:
  std::array<T, N> data;
};

template <typename T, std::size_t N>
auto make_vector(std::array<T, N> arr) -> vector<T, N> {
  return vector<T, N>(arr);
}

template <std::size_t N, typename T>
auto make_vector(const T *ptr) -> vector<T, N> {
  return vector<T, N>(ptr);
}

} // namespace tf
