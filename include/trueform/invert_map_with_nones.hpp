/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./parallel_apply.hpp"
namespace tf {
/// @ingroup algorithms
/// @brief Inverts a mapping with support for missing (none-tagged) entries.
///
/// This function inverts a given mapping by producing a reverse lookup table in `inverse_map`.
/// The original `map` contains indices that map to output positions. Some entries may contain
/// a special value `none_tag`, indicating that the entry should be ignored (i.e., not mapped).
///
/// For each valid index `i` in `map`, such that `map[i] != none_tag`, the function sets:
/// `inverse_map[map[i] - offset] = i;`
///
/// This is commonly used in mesh or index remapping scenarios where only a subset of items
/// (e.g., kept vertices) remain, and one needs to construct the reverse association from
/// the new indices to the original.
///
/// @tparam Range0 Type of the input range (original map).
/// @tparam Range1 Type of the output range (inverse map).
/// @tparam Index Index type, typically `std::size_t` or `int`.
///
/// @param map Input mapping range. Each entry maps an original index to a new index or `none_tag`.
/// @param inverse_map Output range that will hold the inverse mapping.
/// @param none_tag Value indicating a missing mapping (i.e., this index should be ignored).
/// @param offset Optional offset to subtract from the mapped index when writing to `inverse_map`. Defaults to 0.
  template<typename Range0, typename Range1, typename Index>
  auto invert_map_with_nones(const Range0 &map, Range1&&inverse_map, Index none_tag, Index offset = 0) {
    tf::parallel_apply(std::size_t(0), std::size_t(map.size()), [&](std::size_t index){
        if(map[index] != none_tag)
          inverse_map[map[index]] = index + offset;
        });
  }
}
