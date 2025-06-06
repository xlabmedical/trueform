/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./normalized.hpp"
#include "./unsafe.hpp"
#include "./value_type.hpp"
#include "./vector.hpp"

namespace tf {

/// @ingroup geometry
/// @brief A fixed-size unit vector wrapper type.
///
/// `unit_vector<T, N>` represents a vector of dimension `N` with a fixed length
/// of 1. It inherits from `tf::vector<T, N>` but provides guarantees and
/// overrides to reflect the unit-length invariant. All instances must be
/// normalized at construction time.
///
/// Use `make_unit_vector()` or `make_unit_vector_unsafe()` to create instances.
///
/// @tparam T The scalar type (e.g. float, double).
/// @tparam N The number of dimensions (e.g. 2, 3, 4).
template <typename T, std::size_t N> struct unit_vector : tf::vector<T, N> {
  /// @brief Constructs a unit vector by normalizing the input.
  ///
  /// This constructor calls `tf::normalized(v)` internally. It ensures the
  /// resulting vector has unit length.
  ///
  /// @param v A vector that will be normalized.
  explicit unit_vector(const tf::vector<T, N> &v)
      : tf::vector<T, N>{tf::normalized(v)} {}

  /// @brief Constructs a unit vector assuming the input is already normalized.
  ///
  /// This constructor skips normalization and wraps the vector directly. It is
  /// the caller’s responsibility to ensure the input has length 1. Use this
  /// when you are sure of the input and want to avoid redundant computation.
  ///
  /// @param v A vector that is already normalized.
  unit_vector(tf::unsafe_t, const tf::vector<T, N> &v) : tf::vector<T, N>{v} {}

  /// @brief Returns the squared length (always 1).
  /// @return The value `1`.
  constexpr auto length2() const -> T { return 1; }

  /// @brief Returns the length (always 1).
  /// @return The value `1`.
  constexpr auto length() const -> T { return 1; }
};

/// @ingroup geometry
/// @brief Safely construct a unit vector by normalizing the input.
///
/// This function creates a `unit_vector<T, Dims>` from any vector-like input
/// by computing its normalized form.
///
/// @tparam Dims The number of dimensions.
/// @tparam T A type derived from `vector_like`.
/// @param v A vector that will be normalized before being wrapped.
/// @return A `unit_vector` instance with length 1.
template <std::size_t Dims, typename T>
auto make_unit_vector(const tf::vector_like<Dims, T> &v) {
  return unit_vector<tf::value_type<T>, Dims>{v};
}

/// @ingroup geometry
/// @brief Construct a unit vector from an already-normalized input.
///
/// This function creates a `unit_vector<T, Dims>` assuming the input is already
/// normalized. No normalization is performed. Use only if you are sure of the
/// input.
///
/// @tparam Dims The number of dimensions.
/// @tparam T A type derived from `vector_like`.
/// @param v A vector with unit length.
/// @return A `unit_vector` instance wrapping the given vector.
template <std::size_t Dims, typename T>
auto make_unit_vector_unsafe(const tf::vector_like<Dims, T> &v) {
  return unit_vector<tf::value_type<T>, Dims>{tf::unsafe, v};
}

} // namespace tf
