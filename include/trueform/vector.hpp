/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include <array>
#include <cmath>

namespace tf {

/// @brief Fixed-size N-dimensional vector with element-wise arithmetic and
/// comparisons.
///
/// `tf::vector<T, N>` is a general-purpose geometric vector class supporting
/// standard vector algebra (addition, subtraction, scalar multiplication, etc.)
/// and comparisons. It is used throughout the library for spatial coordinates,
/// offsets, and directions.
///
/// The class supports:
/// - Element access via `operator[]`
/// - Component-wise arithmetic (`+`, `-`, `*`, `/`)
/// - Comparisons (`==`, `!=`, `<`, `>` if defined for T)
/// - Iteration and pointer conversion
/// - conversions using as<U>() method
/// - length() method
///
/// Use `tf::make_vector(...)` to construct vectors from raw arrays or pointers.
///
/// @tparam T The scalar element type (e.g., float, double, int).
/// @tparam N The dimensionality (e.g., 2, 3).
template <typename T, std::size_t N> class vector {
public:
  /// @brief Default constructor. Initializes the vector to zero-initialized
  /// state.
  vector() = default;

  /// @brief Constructs a vector from a std::array.
  vector(std::array<T, N> init) : data{init} {};

  /// @brief Constructs a vector from a raw pointer to `N` elements.
  vector(const T *ptr) : data{} {
    for (std::size_t i = 0; i < N; ++i)
      data[i] = ptr[i];
  }

  /// @brief Converts the vector to another vector type if elements are
  /// convertible.
  template <typename U, typename = typename std::enable_if_t<
                            std::is_convertible<T, U>::value, void>>
  operator vector<U, N>() const {
    vector<U, N> out;
    for (std::size_t i = 0; i < N; ++i)
      out[i] = data[i];
    return out;
  }

  /// @brief Returns a new vector converted to another scalar type.
  template <typename U, typename = typename std::enable_if_t<
                            std::is_convertible<T, U>::value, void>>
  auto as() const -> vector<U, N> {
    return *this;
  }

  // Element access

  /// @brief Mutable element access.
  auto operator[](std::size_t i) -> T & { return data[i]; }

  /// @brief Const element access.
  auto operator[](std::size_t i) const -> const T & { return data[i]; }

  /// @brief Returns a const pointer to the beginning of the vector.
  auto begin() const -> const T * { return data.data(); }

  /// @brief Returns a const pointer to the end of the vector.
  auto end() const -> const T * { return data.data() + N; }

  /// @brief Returns a mutable pointer to the beginning of the vector.
  auto begin() -> T * { return data.data(); }

  /// @brief Returns a mutable pointer to the end of the vector.
  auto end() -> T * { return data.data() + N; }

  /// @brief Computes the squared Euclidean length of the vector.
  auto length2() const -> T {
    T l2 = 0;
    for (int i = 0; i < int(N); ++i)
      l2 += data[i] * data[i];
    return l2;
  }

  /// @brief Computes the Euclidean length of the vector.
  auto length() const -> T { return std::sqrt(length2()); }

  // Compound assignment: +=

  /// @brief Adds another vector to this one in-place.
  auto operator+=(const vector &other) -> vector & {
    for (std::size_t i = 0; i < N; ++i)
      data[i] += other[i];
    return *this;
  }

  // Compound assignment: -=

  /// @brief Subtracts another vector from this one in-place.
  auto operator-=(const vector &other) -> vector & {
    for (std::size_t i = 0; i < N; ++i)
      data[i] -= other[i];
    return *this;
  }

  // Compound assignment: *= scalar

  /// @brief Multiplies this vector by a scalar in-place.
  auto operator*=(T scalar) -> vector & {
    for (std::size_t i = 0; i < N; ++i)
      data[i] *= scalar;
    return *this;
  }

  // Compound assignment: /= scalar

  /// @brief Divides this vector by a scalar in-place.
  auto operator/=(T scalar) -> vector & {
    for (std::size_t i = 0; i < N; ++i)
      data[i] /= scalar;
    return *this;
  }

  // Friends using compound forms

  /// @brief Adds two vectors.
  friend auto operator+(vector lhs, const vector &rhs) -> vector {
    return lhs += rhs;
  }

  /// @brief Subtracts one vector from another.
  friend auto operator-(vector lhs, const vector &rhs) -> vector {
    return lhs -= rhs;
  }

  /// @brief Multiplies a vector by a scalar.
  friend auto operator*(vector lhs, T scalar) -> vector {
    return lhs *= scalar;
  }

  /// @brief Multiplies a scalar by a vector.
  friend auto operator*(T scalar, vector rhs) -> vector {
    return rhs *= scalar;
  }

  /// @brief Divides a vector by a scalar.
  friend auto operator/(vector lhs, T scalar) -> vector {
    return lhs /= scalar;
  }

  // Unary minus

  /// @brief Returns the negated vector.
  friend auto operator-(const vector &v) -> vector {
    vector result;
    for (std::size_t i = 0; i < N; ++i)
      result[i] = -v[i];
    return result;
  }

  // Equality

  /// @brief Checks if two vectors are equal.
  friend auto operator==(const vector &a, const vector &b) -> bool {
    return a.data == b.data;
  }

  /// @brief Checks if two vectors are not equal.
  friend auto operator!=(const vector &a, const vector &b) -> bool {
    return !(a == b);
  }

  /// @brief Lexicographical comparison.
  friend auto operator<(const vector &a, const vector &b) -> bool {
    return a.data < b.data;
  }

  /// @brief Lexicographical comparison.
  friend auto operator>(const vector &a, const vector &b) -> bool {
    return a.data > b.data;
  }

  /// @brief Lexicographical comparison.
  friend auto operator<=(const vector &a, const vector &b) -> bool {
    return !(a > b);
  }

  /// @brief Lexicographical comparison.
  friend auto operator>=(const vector &a, const vector &b) -> bool {
    return !(a < b);
  }

private:
  std::array<T, N> data;
};

/// @brief Construct a vector from a `std::array`.
///
/// Creates a @ref tf::vector<T, N> by copying values from the given array.
///
/// @tparam T The scalar element type.
/// @tparam N The dimensionality.
/// @param arr The array to copy values from.
/// @return A `tf::vector<T, N>` initialized from the array.
template <typename T, std::size_t N>
auto make_vector(std::array<T, N> arr) -> vector<T, N> {
  return vector<T, N>(arr);
}

/// @brief Construct a vector from a raw pointer.
///
/// Creates a @ref tf::vector<T, N> by copying `N` elements from the given
/// pointer. The pointer must reference a contiguous array of at least `N`
/// elements.
///
/// @tparam N The dimensionality.
/// @tparam T The scalar element type.
/// @param ptr Pointer to a contiguous block of `N` elements.
/// @return A `tf::vector<T, N>` initialized from the pointer data.
template <std::size_t N, typename T>
auto make_vector(const T *ptr) -> vector<T, N> {
  return vector<T, N>(ptr);
}

} // namespace tf
