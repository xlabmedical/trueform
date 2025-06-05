/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./borrowed_data.hpp"
#include "./vector_like.hpp"

namespace tf {

/// @ingroup geometry
/// @brief Lightweight non-owning view of a fixed-size N-dimensional vector.
///
/// `tf::vector_view<T, N>` provides the same interface as @ref tf::vector,
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
/// Use @ref tf::make_vector_view to create views from raw pointers.
///
/// @tparam T The referenced scalar element type (e.g., float, double, int).
/// @tparam N The dimensionality (e.g., 2, 3).

template <typename T, std::size_t N>
using vector_view = tf::vector_like<N, tf::borrowed_data<T, N>>;

/// @ingroup geometry
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

/// @ingroup geometry
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
