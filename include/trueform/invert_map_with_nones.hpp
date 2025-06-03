/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./parallel_apply.hpp"
namespace tf {
  template<typename Range0, typename Range1, typename Index>
  auto invert_map_with_nones(const Range0 &map, Range1&&inverse_map, Index none_tag, Index offset = 0) {
    tf::parallel_apply(std::size_t(0), std::size_t(map.size()), [&](std::size_t index){
        if(map[index] != none_tag)
          inverse_map[map[index]] = index + offset;
        });
  }
}
