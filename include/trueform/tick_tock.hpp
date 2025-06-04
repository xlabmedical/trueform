/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include <chrono>
#include <iostream>
#include <string_view>

namespace tf {

namespace implementation {

// Global inline tick start point
inline std::chrono::steady_clock::time_point tick_start;
} // namespace implementation

/// @brief Start a timing measurement.
///
/// Captures the current time using `std::chrono::steady_clock`. Typically used in conjunction with @ref tock().
inline auto tick() -> void {
  implementation::tick_start = std::chrono::steady_clock::now();
}

/// @brief End a timing measurement and return the elapsed time in milliseconds.
///
/// Measures the duration since the last call to @ref tick().
///
/// @return Elapsed time in milliseconds as a `float`.
inline auto tock() -> float {
  auto end = std::chrono::steady_clock::now();
  return std::chrono::duration<float, std::milli>(end -
                                                  implementation::tick_start)
      .count();
}

/// @brief End a timing measurement and print the result with a custom message.
///
/// Measures the duration since the last call to @ref tick() and prints it to `std::cout`
/// along with a user-specified message.
///
/// @param msg A label to display before the timing result.
inline auto tock(std::string_view msg) -> void {
  auto time = tock();
  std::cout << msg << " " << time << " ms" << std::endl;
}

} // namespace tf
