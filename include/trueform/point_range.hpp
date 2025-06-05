/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./implementation/point_iterator.hpp"
#include "./range.hpp"

namespace tf {

/// @ingroup ranges
/// @brief Creates a range of points from a flat scalar sequence.
///
/// This utility interprets a flat range of scalars as a sequence of
/// fixed-dimensional points. It constructs a @ref tf::range view over
/// `Dims`-dimensional @ref tf::vector_view elements, where each point
/// occupies `Dims` consecutive scalars in the original range.
///
/// This is especially useful when working with flat buffers of interleaved
/// coordinates, such as geometry loaded from binary files or raw memory layouts.
///
/// @tparam Dims The number of dimensions per point (e.g., 2 or 3).
/// @tparam Range A range type whose elements are scalar values (e.g., float, double).
/// @param r A flat range of scalar values representing interleaved point coordinates.
/// @return A @ref tf::range of @ref tf::vector_view elements, each representing a point.
///
/// @note The size of the returned range is `r.size() / Dims`.
/// @note The input range must contain a total number of elements divisible by `Dims`.
///
/// @code
/// tf::buffer<float> flat{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
/// for (auto pt : make_points_range<3>(flat)) {
///   auto [x, y, z] = pt;
///   std::cout << x << ", " << y << ", " << z << '\n';
/// }
/// // Output:
/// // 1, 2, 3,
/// // 4, 5, 6
/// @endcode
template <std::size_t Dims, typename Range> auto make_point_range(Range &&r) {
  auto begin = tf::implementation::iter::make_point_iterator<Dims>(r.begin());
  auto end = tf::implementation::iter::make_point_iterator<Dims>(r.end());
  return tf::make_range(std::move(begin), std::move(end));
}

} // namespace tf
