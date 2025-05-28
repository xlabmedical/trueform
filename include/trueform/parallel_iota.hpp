/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./parallel_for.hpp"
#include <numeric>

namespace tf {

template <typename Range, typename Val> auto parallel_iota(Range &&r, Val val) {
  parallel_for(r, [val, start = r.begin()](auto begin, auto end) {
    std::iota(begin, end, val + (begin - start));
  });
}

} // namespace tf
