/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./vector.hpp"
#include <cmath>

namespace tf {

namespace implementation {

template <typename T, std::size_t N, bool is_assignable = !std::is_const_v<T>>
class vector_view_base {
public:
  vector_view_base(T *ptr) : data{ptr} {}
  auto operator=(const vector_view_base &other) -> vector_view_base & {
    for (std::size_t i = 0; i < N; ++i)
      data[i] = other.data[i];
    return *this;
  }

protected:
  T *data;
};

template <typename T, std::size_t N> class vector_view_base<T, N, false> {
public:
  vector_view_base(T *ptr) : data{ptr} {}
  auto operator=(const vector_view_base &other) -> vector_view_base & = delete;
  auto operator=(vector_view_base &&other) -> vector_view_base & = delete;

protected:
  T *data;
};
} // namespace implementation


/// @brief Lightweight non-owning view of a fixed-size N-dimensional vector.
///
/// `tf::vector_view<T, N>` provides the same interface as `tf::vector<T, N>`,
/// but wraps a raw pointer instead of owning the underlying data. It is useful
/// for referencing external memory (e.g., interleaved buffers, mapped arrays)
/// without copying or allocating.
///
/// Supports:
/// - Element access via `operator[]`
/// - Component-wise arithmetic (when `T` is mutable)
/// - Comparisons (`==`, `!=`, `<`, `>` if defined for T)
/// - Seamless interop with `tf::vector<T, N>`
/// - conversion to vector using the as<U>() method
/// - length() method
///
/// This class does not perform bounds checking or lifetime management.
/// Use `tf::make_vector_view(...)` to create views from raw pointers.
///
/// @tparam T The referenced scalar element type (e.g., float, double, int).
/// @tparam N The dimensionality (e.g., 2, 3).
template <typename T, std::size_t N>
class vector_view : public implementation::vector_view_base<T, N> {
private:
  using base_t = implementation::vector_view_base<T, N>;

public:
  using base_t::base_t;

  template <typename U,
            typename = std::enable_if_t<std::is_assignable_v<T &, const U &>>>
  auto operator=(const vector<U, N> &other) -> vector_view & {
    for (std::size_t i = 0; i < N; ++i)
      base_t::data[i] = other[i];
    return *this;
  }

  template <typename U,
            typename = std::enable_if_t<std::is_assignable_v<T &, const U &>>>
  auto operator=(const vector_view<U, N> &other)
      -> vector_view & {
    for (std::size_t i = 0; i < N; ++i)
      base_t::data[i] = other[i];
    return *this;
  }

  template <typename U,
            typename = std::enable_if_t<std::is_convertible_v<T, U>, void>>
  operator vector<U, N>() const {
    vector<U, N> out;
    for (std::size_t i = 0; i < N; ++i)
      out[i] = base_t::data[i];
    return out;
  }

  template <typename U, typename = typename std::enable_if_t<
                            std::is_convertible<T, U>::value, void>>
  auto as() const -> vector<U, N> {
    return *this;
  }

  // Element access
  auto operator[](std::size_t i) -> T & { return base_t::data[i]; }
  auto operator[](std::size_t i) const -> const T & { return base_t::data[i]; }

  auto begin() const -> const T * { return base_t::data; }

  auto end() const -> const T * { return base_t::data; }

  auto begin() -> T * { return base_t::data; }

  auto end() -> T * { return base_t::data; }

  auto length2() const -> T {
    T l2 = 0;
    for (int i = 0; i < int(N); ++i)
      l2 += base_t::data[i] * base_t::data[i];
    return l2;
  }

  auto length() const -> T { return std::sqrt(length2()); }

  // Compound assignment: +=
  auto operator+=(const vector_view &other) -> vector_view & {
    for (std::size_t i = 0; i < N; ++i)
      base_t::data[i] += other[i];
    return *this;
  }

  auto operator+=(const vector<T, N> &other) -> vector_view & {
    for (std::size_t i = 0; i < N; ++i)
      base_t::data[i] += other[i];
    return *this;
  }

  // Compound assignment: -=
  auto operator-=(const vector_view &other) -> vector_view & {
    for (std::size_t i = 0; i < N; ++i)
      base_t::data[i] -= other[i];
    return *this;
  }

  auto operator-=(const vector<T, N> &other) -> vector_view & {
    for (std::size_t i = 0; i < N; ++i)
      base_t::data[i] -= other[i];
    return *this;
  }

  // Compound assignment: *= scalar
  auto operator*=(T scalar) -> vector_view & {
    for (std::size_t i = 0; i < N; ++i)
      base_t::data[i] *= scalar;
    return *this;
  }

  // Compound assignment: /= scalar
  auto operator/=(T scalar) -> vector_view & {
    for (std::size_t i = 0; i < N; ++i)
      base_t::data[i] /= scalar;
    return *this;
  }

  // Friends using compound forms

  friend auto operator+(vector_view lhs, const vector_view &rhs)
      -> vector<T, N> {
    return lhs.as<T>() += rhs;
  }

  friend auto operator-(vector_view lhs, const vector_view &rhs)
      -> vector<T, N> {
    return lhs.as<T>() -= rhs;
  }

  friend auto operator*(vector_view lhs, T scalar) -> vector<T, N> {
    return lhs.as<T>() *= scalar;
  }

  friend auto operator*(T scalar, vector_view rhs) -> vector<T, N> {
    return rhs.as<T>() *= scalar;
  }

  friend auto operator/(vector_view lhs, T scalar) -> vector<T, N> {
    return lhs.as<T>() /= scalar;
  }

  // Unary minus
  friend auto operator-(const vector_view &v) -> vector<T, N> {
    return -v.as<T>();
  }

  // Equality
  friend auto operator==(const vector_view &a, const vector_view &b) -> bool {
    for (std::size_t i = 0; i < N; ++i)
      if (a[i] != b[i])
        return false;
    return true;
  }

  friend auto operator!=(const vector_view &a, const vector_view &b) -> bool {
    return !(a == b);
  }

  friend auto operator<(const vector_view &a, const vector_view &b) -> bool {
    for (int i = 0; i < N; i++) {
      if (a[i] < b[i])
        return true;
      if (a[i] > b[i])
        return false;
    }
    return false;
  }

  friend auto operator>(const vector_view &a, const vector_view &b) -> bool {
    for (int i = 0; i < N; i++) {
      if (a[i] > b[i])
        return true;
      if (a[i] < b[i])
        return false;
    }
    return false;
  }

  friend auto operator<=(const vector_view &a, const vector_view &b) -> bool {
    return !(a > b);
  }

  friend auto operator>=(const vector_view &a, const vector_view &b) -> bool {
    return !(a < b);
  }
};


/// @brief Construct a constant vector view from a raw pointer.
///
/// Creates a `tf::vector_view<const T, N>` that wraps a contiguous block
/// of `N` read-only elements.
///
/// @tparam N The dimensionality.
/// @tparam T The scalar element type.
/// @param ptr Pointer to a block of at least `N` contiguous elements.
/// @return A `vector_view<const T, N>` referencing the data.
template <std::size_t N, typename T>
auto make_vector_view(const T *ptr) -> vector_view<const T, N> {
  return vector_view<const T, N>(ptr);
}

/// @brief Construct a mutable vector view from a raw pointer.
///
/// Creates a `tf::vector_view<T, N>` that wraps a contiguous block
/// of `N` writable elements.
///
/// @tparam N The dimensionality.
/// @tparam T The scalar element type.
/// @param ptr Pointer to a block of at least `N` contiguous elements.
/// @return A `vector_view<T, N>` referencing the data.
template <std::size_t N, typename T>
auto make_vector_view(T *ptr) -> vector_view<T, N> {
  return vector_view<T, N>(ptr);
}

} // namespace tf
