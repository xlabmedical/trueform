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

/// @brief Creates a range view over a sequence of blocks defined by offsets.
///
/// This utility allows you to iterate over subranges ("blocks") of a data range.
/// The start and end of each block is defined by two consecutive elements in the
/// `offsets` range. The number of blocks is thus `offsets.size() - 1`.
///
/// This is especially useful for grouping elements like polygons, faces, or variable-sized clusters,
/// where each block may contain a different number of elements.
///
/// @tparam Range0 A range of integral offsets (e.g. `std::vector<size_t>`).
/// @tparam Range1 A range of underlying data values.
/// @param offsets A range of offsets. Must contain `n + 1` elements to define `n` blocks.
/// @param data A range of elements from which the blocks are constructed.
/// @return A range view over blocks of data, where each block is itself a `tf::range`.
///
/// @code
/// tf::buffer<std::size_t> offsets{0, 3, 6};
/// tf::buffer<int> values{1, 2, 3, 4, 5, 6};
/// for (auto block : make_offset_block_range(offsets, values)) {
///   for (int v : block) std::cout << v << ' ';
///   std::cout << '\n';
/// }
/// // Output:
/// // 1 2 3
/// // 4 5 6
/// @endcode
///
/// @note This function returns a view — it does not copy the underlying data.
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
