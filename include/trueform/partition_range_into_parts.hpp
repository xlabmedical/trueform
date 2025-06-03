#pragma once
#include "./range.hpp"
#include "tbb/parallel_invoke.h"

namespace tf {
template <typename Range, typename F0, typename F1>
auto partition_range_into_parts(Range &&range, std::size_t parts,
                                const F0 &partition_range_f, const F1 &apply_f,
                                std::size_t partition_id = 0) -> std::size_t {
  size_t total_size = range.end() - range.begin();
  parts = std::min(parts, static_cast<std::size_t>(total_size));

  if (parts == 1) {
    apply_f(range, partition_id);
    return 1;
  }

  std::size_t left_parts = parts / 2;
  std::size_t right_parts = parts - left_parts;

  size_t mid_size = total_size * left_parts / parts;
  auto mid = range.begin() + mid_size;

  partition_range_f(range.begin(), mid, range.end());

  std::size_t count_left = 0;
  std::size_t count_right = 0;

  if (range.size() > 1000) {
    tbb::parallel_invoke(
        [&]() {
          count_left = partition_range_into_parts(
              tf::make_range(range.begin(), mid), left_parts, partition_range_f,
              apply_f, partition_id);
        },
        [&]() {
          count_right = partition_range_into_parts(
              tf::make_range(mid, range.end()), right_parts, partition_range_f,
              apply_f, partition_id + left_parts);
        });
  } else {
    count_left = partition_range_into_parts(tf::make_range(range.begin(), mid),
                                            left_parts, partition_range_f,
                                            apply_f, partition_id);
    count_right = partition_range_into_parts(
        tf::make_range(mid, range.end()), right_parts, partition_range_f, apply_f,
        partition_id + left_parts);
  }

  return count_left + count_right;
}
} // namespace tf
