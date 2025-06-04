/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./indirect_range.hpp"
#include "./mapped_range.hpp"
#include "./range.hpp"
namespace tf {
namespace implementation {
template <typename Range> struct block_indirect_policy {
  Range range;
  template <typename Range1> auto operator()(Range1 &&ids) const {
    return tf::make_indirect_range(ids, range);
  }
};
} // namespace implementation

template <typename Range0, typename Range1>
auto make_block_indirect_range(Range0 &&blocks, Range1 &&data) {
  auto data_view = tf::make_range(data);
  return tf::make_mapped_range(
      static_cast<Range0 &&>(blocks),
      implementation::block_indirect_policy<decltype(data_view)>{
          std::move(data_view)});
}
} // namespace tf
