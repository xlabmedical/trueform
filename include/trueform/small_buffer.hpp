/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./implementation/small_vector.hpp"
namespace tf {
template <typename T, unsigned N>
using small_buffer = tf::implementation::llvm_vecsmall::SmallVector<T, N>;
}
