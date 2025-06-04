/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./implementation/mapped_iterator.hpp"
#include "./range.hpp"
namespace tf {
namespace implementation {
template <typename Range> struct offset_block_policy {
  Range range;
  template <typename Iterator> auto operator()(Iterator start) const {
    return tf::make_range(range.begin() + *start,
                          range.begin() + *std::next(start));
  }
};
} // namespace implementation

template <typename Range0, typename Range1>
auto make_offset_block_range(Range0 &&offsets, Range1 &&data) {
  auto data_view = tf::make_range(data);
  auto begin = tf::implementation::iter::make_iter_mapped(
      offsets.begin(),
      implementation::offset_block_policy<decltype(data_view)>{data_view});
  auto end = tf::implementation::iter::make_iter_mapped(
      offsets.size() ? std::prev(offsets.end()) : offsets.end(),
      implementation::offset_block_policy<decltype(data_view)>{data_view});
  return tf::make_range(std::move(begin), std::move(end));
}
} // namespace tf
