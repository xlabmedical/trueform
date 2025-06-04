/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./implementation/mapped_iterator.hpp"
#include "./range.hpp"
#include "./static_size.hpp"

namespace tf {
template <typename Range, typename F> auto make_mapped_range(Range &&r, F &&f) {
  auto begin = tf::implementation::iter::make_mapped(r.begin(), f);
  auto end =
      tf::implementation::iter::make_mapped(r.end(), static_cast<F &&>(f));
  return tf::make_range<tf::static_size_v<std::decay_t<Range>>>(
      std::move(begin), std::move(end));
}
} // namespace tf
