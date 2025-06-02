/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include <algorithm>
#include <iterator>
#include <limits>

namespace tf {
template <typename RandomIt> class tree_knn {
  using tree_closest_pt_t = typename std::iterator_traits<RandomIt>::value_type;
  using element_t = typename tree_closest_pt_t::element_t;
  using real_t = typename tree_closest_pt_t::real_t;
  using closest_point_t = typename tree_closest_pt_t::closest_point_t;

public:
  tree_knn(RandomIt out, std::size_t k, real_t radius)
      : out(out), k(k), count(0), worst_metric{radius * radius} {}

  tree_knn(RandomIt out, std::size_t k)
      : out(out), k(k), count(0),
        worst_metric{std::numeric_limits<real_t>::max()} {}

  auto update(element_t element, const closest_point_t &point) -> void {
    if (count < k) {
      out[count++] = tree_closest_pt_t{element, point};
      std::inplace_merge(
          out, out + count - 1, out + count,
          [](const auto &a, const auto &b) { return a.metric() < b.metric(); });
      worst_metric = out[count - 1].metric();
    } else if (point.metric < worst_metric) {
      auto it = std::upper_bound(out, out + k - 1, point.metric,
                                 [](const auto &value, const auto &elem) {
                                   return value < elem.metric();
                                 });
      std::move_backward(it, out + k - 1, out + k);
      *it = tree_closest_pt_t{element, point};
      worst_metric = out[k - 1].metric();
    }
  }

  auto metric() const -> real_t { return worst_metric; }

  auto begin() const -> RandomIt { return out; }

  auto end() const -> RandomIt { return out + count; }

  auto capacity() const -> RandomIt { return out + k; }

  auto size() const -> std::size_t { return count; }

  auto empty() const -> bool { return count == 0; }

private:
  RandomIt out;
  std::size_t k;
  std::size_t count;
  real_t worst_metric;
};

template <typename RandomIt>
auto make_tree_knn(RandomIt iterator, std::size_t k) {
  return tree_knn<RandomIt>{iterator, k};
}

template <typename RandomIt, typename RealT>
auto make_tree_knn(RandomIt iterator, std::size_t k, RealT radius) {
  return tree_knn<RandomIt>{iterator, k, radius};
}
} // namespace tf
