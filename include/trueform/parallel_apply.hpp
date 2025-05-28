/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "tbb/parallel_for.h"

namespace tf {
template <typename Iterator, typename Func>
auto parallel_apply(Iterator first, Iterator last, Func &&f) -> void {
  tbb::parallel_for(
      tbb::blocked_range<Iterator>(first, last),
      [f = static_cast<Func &&>(f)](const tbb::blocked_range<Iterator> &range) {
        for (Iterator it = range.begin(); it != range.end(); ++it) {
          f(*it);
        }
      });
}

template <typename Range, typename Func>
auto parallel_apply(Range &&r, Func &&f) -> void {
  return parallel_apply(r.begin(), r.end(), static_cast<Func &&>(f));
}

template <typename Iterator0, typename Iterator1, typename Func>
auto parallel_apply(std::tuple<Iterator0, Iterator1> first,
                    std::pair<Iterator0, Iterator1> last, Func &&f) -> void {
  tbb::parallel_for(
      tbb::blocked_range<std::size_t>(0, last - first),
      [f = static_cast<Func &&>(f),
       first](const tbb::blocked_range<std::size_t> &range) {
        for (std::size_t i = range.begin(); i < range.end(); ++i) {
          f(std::forward_as_tuple(*(first.first + i), *(first.second + i)));
        }
      });
}

template <typename Range0, typename Range1, typename Func>
auto parallel_apply(std::tuple<Range0, Range1> r, Func &&f) -> void {
  tbb::parallel_for(tbb::blocked_range<std::size_t>(0, std::get<0>(r).size()),
                    [f = static_cast<Func &&>(f),
                     &r](const tbb::blocked_range<std::size_t> &range) {
                      for (std::size_t i = range.begin(); i < range.end(); ++i)
                        f(std::get<0>(r)[i], std::get<1>(r)[i]);
                    });
}
} // namespace tf
