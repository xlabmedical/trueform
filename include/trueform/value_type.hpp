/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include <type_traits>

namespace tf {
namespace implementation {
template <typename T, bool = std::is_fundamental<T>::value>
struct get_value_type {
  using type = typename T::value_type;
};

template <typename T> struct get_value_type<T, true> {
  using type = T;
};
} // namespace implementation

template <typename T>
using value_type = typename implementation::get_value_type<T>::type;

template <typename... Ts>
using common_value = std::common_type_t<value_type<std::decay_t<Ts>>...>;

} // namespace tf
