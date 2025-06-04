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
template <typename Iterator, std::size_t N> class indirect_range;

template <typename Iterator, typename RandomIterator, std::size_t N>
class indirect_range<implementation::iter::indirect<Iterator, RandomIterator>,
                     N>
    : public range<implementation::iter::indirect<Iterator, RandomIterator>,
                   N> {
  using base_t =
      range<implementation::iter::indirect<Iterator, RandomIterator>, N>;

public:
  using base_t::base_t;

  auto ids() const {
    if constexpr (N == tf::dynamic_size)
      return tf::make_range(base_t::begin().base_iter(),
                            base_t::end().base_iter());
    else
      return tf::make_range<N>(base_t::begin().base_iter());
  }
};

template <typename Iterator, std::size_t N>
struct static_size<indirect_range<Iterator, N>>
    : std::integral_constant<std::size_t, N> {};

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
