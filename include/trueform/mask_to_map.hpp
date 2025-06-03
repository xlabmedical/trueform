/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include <type_traits>

namespace tf {
template <typename Range0, typename Range1, typename Index>
auto mask_to_map(const Range0 &mask, const Range1 &&map,
                 const Index &none_tag) {
  Index current_id = 0;
  auto it0 = mask.begin();
  auto end0 = mask.end();
  auto it1 = map.begin();
  for (; it0 != end0; ++it0, ++it1) {
    if (*it0)
      *it1 = current_id++;
    else
      *it1 = none_tag;
  }
  return current_id;
}

template <typename Range0, typename Range1>
auto mask_to_map(const Range0 &mask, const Range1 &&map) {
  using index_t = std::decay_t<decltype(map[0])>;
  index_t none_tag = map.size();
  return mask_to_map(map, mask, none_tag);
}
} // namespace tf
