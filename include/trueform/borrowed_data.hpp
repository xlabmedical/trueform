/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */

#pragma once
#include <cstddef>

namespace tf {
template <typename T, std::size_t Dims> struct borrowed_data {
  using element_type = T;
  using value_type = T;

  explicit borrowed_data(T *ptr) : _data(ptr) {}
  borrowed_data(const borrowed_data &) = default;
  borrowed_data(borrowed_data &&) = default;

  auto operator=(const borrowed_data &other) -> borrowed_data & {
    for (std::size_t i = 0; i < Dims; ++i)
      _data[i] = other._data[i];
    return *this;
  }

  auto operator=(borrowed_data &&other) -> borrowed_data & {
    for (std::size_t i = 0; i < Dims; ++i)
      _data[i] = other._data[i];
    return *this;
  }

  template <typename Point>
  auto operator=(const Point &other) -> borrowed_data & {
    for (std::size_t i = 0; i < Dims; ++i)
      _data[i] = other[i];
    return *this;
  }

  auto data() -> T * { return _data; }
  auto data() const -> const T * { return _data; }

private:
  T *_data;
};

template <typename T, std::size_t Size> struct borrowed_data<const T, Size> {
  using element_type = const T;
  using value_type = T;

  borrowed_data() = default;
  explicit borrowed_data(const T *ptr) : _data(ptr) {}
  borrowed_data(const borrowed_data &) = default;
  borrowed_data(borrowed_data &&) = default;

  auto operator=(const borrowed_data &other) -> borrowed_data & = delete;
  auto operator=(borrowed_data &&other) -> borrowed_data & = delete;

  auto data() const -> const T * { return _data; }

private:
  const T *_data;
};
} // namespace tf
