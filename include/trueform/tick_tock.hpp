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

inline auto tick() -> void {
  implementation::tick_start = std::chrono::steady_clock::now();
}

inline auto tock() -> float {
  auto end = std::chrono::steady_clock::now();
  return std::chrono::duration<float, std::milli>(end -
                                                  implementation::tick_start)
      .count();
}

inline auto tock(std::string_view msg) -> void {
  auto time = tock();
  std::cout << msg << " " << time << " ms" << std::endl;
}
} // namespace tf
