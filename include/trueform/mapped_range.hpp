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
/// @ingroup ranges
/// @brief Creates a mapped view over an input range by applying a transformation function.
///
/// This function constructs a non-owning, lazy view where each element is the result of
/// applying the provided function `f` to the corresponding element of the input range `r`.
/// 
/// The resulting view does not allocate or own memory. It references the underlying data
/// and applies the mapping function on demand.
///
/// If the input range has a static size (determined via @ref static_size_v), that size
/// is propagated to the resulting view. This allows the view to preserve compile-time
/// size information when possible.
///
/// @tparam Range A range type with `begin()` and `end()` iterators.
/// @tparam F A function or functor that can be applied to elements of the range.
/// @param r The input range to be wrapped.
/// @param f The transformation function to apply to each element.
/// @return A mapped view over the input range with propagated static size.
///
/// @see @ref make_range
/// @see @ref static_size_v
template <typename Range, typename F> auto make_mapped_range(Range &&r, F &&f) {
  auto begin = tf::implementation::iter::make_mapped(r.begin(), f);
  auto end =
      tf::implementation::iter::make_mapped(r.end(), static_cast<F &&>(f));
  return tf::make_range<tf::static_size_v<std::decay_t<Range>>>(
      std::move(begin), std::move(end));
}
} // namespace tf
