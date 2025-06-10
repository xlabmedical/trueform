/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./mapped_range.hpp"
#include "./polygon.hpp"
namespace tf {
namespace implementation {
template <typename Range0> struct polygon_range_policy {
  Range0 points;
  template <typename Range> auto operator()(Range &&ids) const {
    return tf::make_polygon(ids, points);
  }
};
} // namespace implementation

template <typename Range0, typename Range1>
struct polygon_range
    : decltype(tf::make_mapped_range(
          std::declval<Range0>(),
          std::declval<implementation::polygon_range_policy<Range1>>())) {
private:
  using base_t = decltype(tf::make_mapped_range(
      std::declval<Range0>(),
      std::declval<implementation::polygon_range_policy<Range1>>()));

public:
  polygon_range(const Range0 &faces, const Range1 &points)
      : base_t{tf::make_mapped_range(
            faces, implementation::polygon_range_policy<Range1>{points})} {}

  auto faces() const {
    return tf::make_range(base_t::begin().base_iter().base_iter(),
                          base_t::size());
  }

  auto points() const {
    return tf::make_range(
        base_t::begin().base_iter().dereference_policy().range);
  }
};

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
/// @tparam Range0 A range of integral indices grouped into blocks (e.g.,
/// faces).
/// @tparam Range1 A range of point data (e.g., `std::vector<vec2>`).
/// @param faces A range defining groups of indices, where each group forms a
/// polygon.
/// @param points A range of points from which polygons are constructed.
/// @return A @ref tf::range of @ref tf::polygon objects.
///
/// @note Internally uses @ref tf::make_block_indirect_range and
/// @ref tf::make_mapped_range with @ref
/// tf::implementation::polygon_range_policy.
/// @note This function returns a view — it does not copy the underlying data.
///
/// @code
/// tf::buffer<std::size_t> faces{0, 3, 6};
/// tf::buffer<std::size_t> ids{0, 1, 2, 2, 3, 0};
/// tf::buffer<vec2> points{{0,0}, {1,0}, {1,1}, {0,1}};
/// for (auto poly : make_polygon_range(make_offset_block_range(faces, ids),
/// points)) {
///   draw(poly);
/// }
/// @endcode
template <typename Range0, typename Range1>
auto make_polygon_range(Range0 &&faces, Range1 &&points) {
  auto r0 = tf::make_range(faces);
  auto r1 = tf::make_range(points);
  return polygon_range<decltype(r0), decltype(r1)>{r0, r1};
}
} // namespace tf
