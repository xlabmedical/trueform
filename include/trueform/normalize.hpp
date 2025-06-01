#pragma once
#include "./vector.hpp"

namespace tf {
template <typename T, std::size_t Dims>
auto normalize(tf::vector<T, Dims> &v) -> tf::vector<T, Dims> & {
  auto d = v.length();
  v /= d + (d == 0);
  return v;
}
} // namespace tf
