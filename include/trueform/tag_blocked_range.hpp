/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./blocked_range.hpp"
#include "./mapped_range.hpp"
#include "./tagged_range.hpp"

namespace tf {
namespace implementation {
struct tag_blocked_policy {
  template <typename Range> auto operator()(Range &&r) const {
    return tf::make_tagged_range(static_cast<Range &&>(r));
  }
};
} // namespace implementation
/// @ingroup ranges
/// @brief Creates a view over a range where elements are grouped into
/// consecutive blocks, and a tag preceeding them, with compile-time block size.
///
/// This overload provides a static block size via the template parameter
/// `BlockSize`, which can enable additional optimizations and compile-time
/// checks.
///
/// The resulting range is a non-owning view that yields blocks of size
/// `BlockSize`, each presented as a subrange. For example, a buffer `[tag_0, 0,
/// 1, 2, tag_1, 3, 4, 5]` with `BlockSize = 3` yields two blocks: `[0, 1, 2]`
/// and `[3, 4, 5]`, with tags `tag_0` and `tag_1`
///
/// @tparam BlockSize The number of elements per block (known at compile time).
/// @tparam Range The type of the input range.
/// @param r The input range to be grouped.
/// @return A view over the input range with elements grouped into tagged_blocks
/// of size `BlockSize`.
/// @see @ref tf::tagged_range
template <std::size_t BlockSize, typename Range>
auto make_tag_blocked_range(Range &&range) {
  return tf::make_mapped_range(tf::make_blocked_range<BlockSize + 1>(range),
                               implementation::tag_blocked_policy{});
}
/// @ingroup ranges
/// @brief Creates a view over a range where elements are grouped into
/// consecutive blocks, and a tag preceeding them.
///
/// This overload provides a dynamic block size.
///
/// The resulting range is a non-owning view that yields blocks of size
/// `BlockSize`, each presented as a subrange. For example, a buffer `[tag_0, 0,
/// 1, 2, tag_1, 3, 4, 5]` with `BlockSize = 3` yields two blocks: `[0, 1, 2]`
/// and `[3, 4, 5]`, with tags `tag_0` and `tag_1`
///
/// @tparam Range The type of the input range.
/// @param r The input range to be grouped.
/// @param Size of the blocks
/// @return A view over the input range with elements grouped into blocks of
/// size `block_size`.
template <typename Range>
auto make_tag_blocked_range(Range &&range, std::size_t block_size) {
  return tf::make_mapped_range(tf::make_blocked_range(range, block_size + 1),
                               implementation::tag_blocked_policy{});
}

} // namespace tf
