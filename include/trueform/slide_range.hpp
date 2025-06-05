/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./implementation/mapped_iterator.hpp"
#include "./range.hpp"

namespace tf {
namespace implementation {
template <std::size_t WindowSize> struct slide_policy {
  template <typename Iterator> auto operator()(Iterator start) const {
    return tf::make_range<WindowSize>(start);
  }
};
template <> struct slide_policy<tf::dynamic_size> {
  std::size_t window_size;
  template <typename Iterator> auto operator()(Iterator start) const {
    return tf::make_range(start, window_size);
  }
};
} // namespace implementation

/// @ingroup ranges
/// @brief Creates a sliding window view over a range with dynamic window size.
///
/// This utility allows iteration over overlapping subranges ("windows") of a
/// given range. Each window is a contiguous slice of the input range of size
/// `window_size`. The windows are computed by sliding one element at a time
/// from the beginning of the range until the end is reached (excluding partial
/// windows).
///
///
/// @tparam Range A range type with random access iterators.
/// @param range The input range to be windowed.
/// @param window_size The number of elements in each window.
/// @return A range of sliding windows, each represented as a @ref tf::range.
///
/// @code
/// tf::buffer<int> values{1, 2, 3, 4};
/// for (auto window : make_slide_range(values, 2)) {
///   for (int v : window) std::cout << v << ' ';
///   std::cout << '\n';
/// }
/// // Output:
/// // 1 2
/// // 2 3
/// // 3 4
/// @endcode
///
/// @note This function returns a view — it does not copy the underlying data.
/// @note Only full windows are included; the number of windows is `range.size()
/// - window_size + 1`.
template <typename Range>
auto make_slide_range(Range &&range, std::size_t window_size) {
  auto begin = tf::implementation::iter::make_iter_mapped(
      range.begin(),
      implementation::slide_policy<tf::dynamic_size>{window_size});
  auto end = tf::implementation::iter::make_iter_mapped(
      range.end() - window_size + 1,
      implementation::slide_policy<tf::dynamic_size>{window_size});
  return tf::make_range(std::move(begin), std::move(end));
}

/// @ingroup ranges
/// @brief Creates a sliding window view over a range with static window size.
///
/// This overload is a compile-time optimized version of @ref make_slide_range,
/// where the window size is known at compile time. It enables more efficient
/// iteration when the window size is fixed.
///
/// @code
/// tf::buffer<int> values{1, 2, 3, 4};
/// for (auto [e0, e1] : make_slide_range<2>(values)) {
///   std::cout << e0 << ", " << e1 << std::endl;
/// }
/// // Output:
/// // 1 2
/// // 2 3
/// // 3 4
/// @endcode
///
/// @tparam WindowSize The static number of elements in each window.
/// @tparam Range A range type with random access iterators.
/// @param range The input range to be windowed.
/// @return A range of sliding windows, each represented as a @ref tf::range.
///
/// @note The number of windows is `range.size() - WindowSize + 1`.
/// @note Returns a view — does not copy the underlying data.
template <std::size_t WindowSize, typename Range>
auto make_slide_range(Range &&range) {
  auto begin = tf::implementation::iter::make_iter_mapped(
      range.begin(), implementation::slide_policy<WindowSize>{});
  auto end = tf::implementation::iter::make_iter_mapped(
      range.end() - WindowSize + 1, implementation::slide_policy<WindowSize>{});
  return tf::make_range(std::move(begin), std::move(end));
}

} // namespace tf
