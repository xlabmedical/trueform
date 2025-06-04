#pragma once
#include "./invert_map_with_nones.hpp"
#include "./mapping.hpp"
#include "./mask_to_map.hpp"
namespace tf {
template <typename Range, typename Index>
auto mask_to_mapping(const Range &mask, tf::mapping_val<Index> &mapping) {
  mapping.f().allocate(mask.size());
  auto n_kept = tf::mask_to_map(mask, mapping.f());
  mapping.kept_ids().allocate(n_kept);
  tf::invert_map_with_nones(mapping.f(), mapping.kept_ids(),
                            Index(mask.size()));
}
template <typename Index, typename Range>
auto mask_to_mapping(const Range &mask) {
  tf::mapping_val<Index> mapping;
  tf::mask_to_mapping(mask, mapping);
  return mapping;
}
} // namespace tf
