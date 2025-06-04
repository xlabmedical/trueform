/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./implementation/indirect_iterator.hpp"
#include "./range.hpp"
#include "./static_size.hpp"

namespace tf {


template <typename Iterator, std::size_t N>
class indirect_range;

/// @brief A view that indirectly accesses elements of a data range using an index range.
///
/// This range uses an iterator over indices to dereference into another data range,
/// enabling indirection-based access patterns. This is useful for operations like
/// reordering or filtering without modifying the original data.
///
/// Inherits from `tf::range`, and allows access to the underlying ID range via `ids()`.
///
/// @tparam Iterator The index iterator type.
/// @tparam RandomIterator The data iterator type.
/// @tparam N Static size or `tf::dynamic_size`.
template <typename Iterator, typename RandomIterator, std::size_t N>
class indirect_range<implementation::iter::indirect<Iterator, RandomIterator>,
                     N>
    : public range<implementation::iter::indirect<Iterator, RandomIterator>,
                   N> {
  using base_t =
      range<implementation::iter::indirect<Iterator, RandomIterator>, N>;

public:
  using base_t::base_t;

  /// @brief Returns a range over the index IDs used for indirection.
  ///
  /// If the indirect range has static size `N`, the returned ID range is also statically sized.
  /// Otherwise, a dynamic range is returned.
  auto ids() const {
    if constexpr (N == tf::dynamic_size)
      return tf::make_range(base_t::begin().base_iter(),
                            base_t::end().base_iter());
    else
      return tf::make_range<N>(base_t::begin().base_iter());
  }
};

/// @brief Specialization of `tf::static_size` for `tf::indirect_range`.
///
/// Allows `indirect_range` to propagate its static size through the type system.
template <typename Iterator, std::size_t N>
struct static_size<indirect_range<Iterator, N>>
    : std::integral_constant<std::size_t, N> {};

/// @brief Creates an `indirect_range` that views `data` using indices from `ids`.
///
/// @tparam Range0 A range of integral indices.
/// @tparam Range1 A range of data values to be accessed indirectly.
/// @param ids A range of indices (e.g. `std::vector<std::size_t>`).
/// @param data A range of values to be indirectly accessed.
/// @return A `tf::indirect_range` using the given index and data ranges.
///
/// @note The resulting range is a view — it does not copy or own the underlying data.
/// @note The static size is propagated using `@ref tf::static_size`.
template <typename Range0, typename Range1>
auto make_indirect_range(Range0 &&ids, Range1 &&data) {
  auto begin =
      tf::implementation::iter::make_indirect(ids.begin(), data.begin());
  auto end = tf::implementation::iter::make_indirect(ids.end(), data.end());
  return indirect_range<decltype(begin),
                        tf::static_size_v<std::decay_t<Range0>>>{
      std::move(begin), std::move(end)};
}
} // namespace tf

namespace std {
template <typename Iterator, std::size_t N>
struct tuple_size<tf::indirect_range<Iterator, N>>
    : tuple_size<tf::range<Iterator, N>> {};
template <std::size_t I, typename Iterator, std::size_t N>
struct tuple_element<I, tf::indirect_range<Iterator, N>>
    : tuple_element<I, tf::range<Iterator, N>> {};

template <std::size_t I, typename Iterator>
struct tuple_element<I, tf::indirect_range<Iterator, tf::dynamic_size>>;

template <std::size_t I, typename Iterator>
struct tuple_element<I, tf::indirect_range<Iterator, tf::dynamic_size>>;
} // namespace std
