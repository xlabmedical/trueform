/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./implementation/value_iterator.hpp"
#include "./range.hpp"
namespace tf {
template <typename T> auto make_sequence_range(T start, T end) {
  static_assert(std::is_integral_v<T>, "T must be integral.");
  auto begin_it = tf::implementation::iter::make_value(start);
  auto end_it = tf::implementation::iter::make_value(end);
  return tf::make_range(std::move(begin_it), std::move(end_it));
}

template <typename T> auto make_sequence_range(T end) {
  static_assert(std::is_integral_v<T>, "T must be integral.");
  auto begin_it = tf::implementation::iter::make_value(T(0));
  auto end_it = tf::implementation::iter::make_value(end);
  return tf::make_range(std::move(begin_it), std::move(end_it));
}
} // namespace tf
