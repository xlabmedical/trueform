/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./normalized.hpp"
#include "./owned_data.hpp"
#include "./unit_vector_like.hpp"
#include "./unsafe.hpp"
#include "./value_type.hpp"

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
template <typename T, std::size_t N>
struct unit_vector : tf::unit_vector_like<N, tf::owned_data<T, N>> {

  unit_vector() {
    for (std::size_t i = 1; i < N; ++i)
      (*this)[i] = 0;
    (*this)[0] = 1;
  }

  template <typename U>
  unit_vector(tf::unsafe_t, const tf::vector_like<N, U> &v) {
    for (std::size_t i = 0; i < N; ++i)
      (*this)[i] = v[i];
  }

  template <typename U> unit_vector(const tf::unit_vector_like<N, U> &v) {
    for (std::size_t i = 0; i < N; ++i)
      (*this)[i] = v[i];
  }

  template <typename U> explicit unit_vector(const tf::vector_like<N, U> &v) {
    auto normed = tf::normalized(v);
    for (std::size_t i = 0; i < N; ++i) {
      (*this)[i] = normed[i];
    }
  }
  template <typename U>
  auto operator=(const tf::unit_vector_like<N, U> &v) -> unit_vector & {
    for (std::size_t i = 0; i < N; ++i)
      (*this)[i] = v[i];
    return *this;
  }
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

template <std::size_t Dims, typename T>
auto make_unit_vector(const tf::unit_vector_like<Dims, T> &v) {
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
