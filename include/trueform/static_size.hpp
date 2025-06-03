/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include <array>
#include <cstddef>

namespace tf {
static constexpr std::size_t dynamic_size = -1;

template <typename T>
struct static_size : std::integral_constant<std::size_t, tf::dynamic_size> {};

template <typename T>
static constexpr std::size_t static_size_v = static_size<T>::value;

template <typename T, std::size_t N>
struct static_size<std::array<T, N>> : std::integral_constant<std::size_t, N> {
};
} // namespace tf
