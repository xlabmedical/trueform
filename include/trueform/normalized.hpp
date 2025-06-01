#pragma once
#include "./normalize.hpp"

namespace tf {
template <typename T, std::size_t Dims>
auto normalized(const tf::vector<T, Dims> &v) -> tf::vector<T, Dims> {
  auto out = v;
  return normalize(out);
}
} // namespace tf
