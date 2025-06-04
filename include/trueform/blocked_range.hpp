/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./implementation/blocked_iterator.hpp"
#include "./range.hpp"

namespace tf {
template <typename Range>
auto make_blocked_range(Range &&r, std::size_t block_size) {
  auto begin =
      tf::implementation::iter::make_blocked_iterator(r.begin(), block_size);
  auto end =
      tf::implementation::iter::make_blocked_iterator(r.end(), block_size);
  return tf::make_range(std::move(begin), std::move(end));
}

template <std::size_t BlockSize, typename Range>
auto make_blocked_range(Range &&r) {
  auto begin =
      tf::implementation::iter::make_blocked_iterator<BlockSize>(r.begin());
  auto end =
      tf::implementation::iter::make_blocked_iterator<BlockSize>(r.end());
  return tf::make_range(std::move(begin), std::move(end));
}
} // namespace tf
