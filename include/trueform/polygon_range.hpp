/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./block_indirect_range.hpp"
#include "./mapped_range.hpp"
#include "./polygon.hpp"
namespace tf {
namespace implementation {
struct polygon_range_policy {
  template <typename Range> auto operator()(Range &&r) const {
    return tf::make_polygon(r);
  }
};
} // namespace implementation

/// @ingroup ranges
/// @brief Creates a range of polygons from indexed face and point data.
///
/// This utility constructs a range of @ref tf::polygon objects using a
/// block-indirect representation: each element in the `faces` range defines a
/// subrange of point indices, which are used to extract corresponding
/// points from the `points` range.
///
/// Each polygon is thus formed by mapping a contiguous block of indices
/// to actual point coordinates.
///
/// @tparam Range0 A range of integral indices grouped into blocks (e.g., faces).
/// @tparam Range1 A range of point data (e.g., `std::vector<vec2>`).
/// @param faces A range defining groups of indices, where each group forms a polygon.
/// @param points A range of points from which polygons are constructed.
/// @return A @ref tf::range of @ref tf::polygon objects.
///
/// @note Internally uses @ref tf::make_block_indirect_range and
/// @ref tf::make_mapped_range with @ref tf::implementation::polygon_range_policy.
/// @note This function returns a view — it does not copy the underlying data.
///
/// @code
/// tf::buffer<std::size_t> faces{0, 3, 6};
/// tf::buffer<std::size_t> ids{0, 1, 2, 2, 3, 0};
/// tf::buffer<vec2> points{{0,0}, {1,0}, {1,1}, {0,1}};
/// for (auto poly : make_polygon_range(make_offset_block_range(faces, ids), points)) {
///   draw(poly);
/// }
/// @endcode
template <typename Range0, typename Range1>
auto make_polygon_range(Range0 &&faces, Range1 &&points) {
  return tf::make_mapped_range(tf::make_block_indirect_range(faces, points),
                               implementation::polygon_range_policy{});
}
} // namespace tf
