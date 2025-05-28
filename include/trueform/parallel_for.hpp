/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "tbb/parallel_for.h"

namespace tf {
template <typename Iterator, typename Func>
auto parallel_for(Iterator first, Iterator last, Func &&f) -> void {
  tbb::parallel_for(
      tbb::blocked_range<Iterator>(first, last),
      [f = static_cast<Func &&>(f)](const tbb::blocked_range<Iterator> &range) {
        f(range.begin(), range.end());
      });
}

template <typename Range, typename Func>
auto parallel_for(Range &&r, Func &&f) -> void {
  return parallel_for(r.begin(), r.end(), static_cast<Func &&>(f));
}
} // namespace tf
