/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./normalized.hpp"
#include "./unsafe.hpp"

namespace tf {

/// @ingroup geometry
/// @brief A fixed-size unit vector wrapper type.
///
/// `unit_vector_like<N, T>` represents a vector of dimension `N` with a fixed
/// length of 1. It inherits from `tf::vector_like<N, T>` but provides
/// guarantees and overrides to reflect the unit-length invariant. All instances
/// must be normalized at construction time.
///
/// Use `make_unit_vector_like()` or `make_unit_vector_unsafe()` to create
/// instances.
///
/// @tparam T The scalar type (e.g. float, double).
/// @tparam N The number of dimensions (e.g. 2, 3, 4).
template <std::size_t Dims, typename T>
struct unit_vector_like : tf::vector_like<Dims, T> {
private:
  template <typename U>
  using value_vec_type = unit_vector_like<Dims, owned_data<U, Dims>>;

public:
  /// @brief Converts this vector to another value type `U`.
  /// @tparam U The target scalar type.
  /// @return A new vector_like instance with elements converted to `U`.
  template <typename U> auto as() const -> value_vec_type<U> {
    return value_vec_type<U>{
        static_cast<const tf::vector_like<Dims, T> &>(*this)};
  }

  /// @brief Implicit conversion to vector of another type.
  template <typename U> operator value_vec_type<U>() const { return as<U>(); }

  friend auto operator-(const unit_vector_like &a) {
    return value_vec_type<typename unit_vector_like::value_type>(
        -static_cast<const tf::vector_like<Dims, T> &>(a));
  }

  /// @brief Returns the squared length (always 1).
  /// @return The value `1`.
  constexpr auto length2() const -> T { return 1; }

  /// @brief Returns the length (always 1).
  /// @return The value `1`.
  constexpr auto length() const -> T { return 1; }

  // forbid asignment operator from base
  template <typename U>
  friend auto operator+=(unit_vector_like &a, const U &b)
      -> unit_vector_like & = delete;
  template <typename U>
  friend auto operator-=(unit_vector_like &a, const U &b)
      -> unit_vector_like & = delete;
  template <typename U>
  friend auto operator*=(unit_vector_like &a, const U &b)
      -> unit_vector_like & = delete;
  template <typename U>
  friend auto operator/=(unit_vector_like &a, const U &b)
      -> unit_vector_like & = delete;
};

} // namespace tf
