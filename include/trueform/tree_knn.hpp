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

/// @brief Helper structure for k-nearest-neighbor (k-NN) queries in spatial trees.
///
/// `tf::tree_knn` maintains a sorted buffer of up to `k` nearest elements during spatial traversal.
/// It is used internally by `tf::nearness_search` when the result is a ranked set of closest elements.
/// The structure may be iterated over like a range.
///
/// Users must supply a random-access iterator to a preallocated buffer of either:
/// - `tf::tree_closest_point<Index, RealT, Dims>`
/// - `tf::tree_closest_point_pair<Index, RealT, Dims>`,
/// depending on the query.
///
/// It supports optional radius-limited queries, which discard elements beyond the specified threshold.
///
/// Use `tf::make_tree_knn` to create an instance.
///
/// @tparam RandomIt A random-access iterator pointing to a buffer of `tf::tree_closest_point` or `tf::tree_closest_point_pair`.
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

/// @brief Return the current worst (farthest) distance among stored results.
///
/// This value represents the squared distance of the farthest element in the current set.
/// It is used during traversal to determine if a candidate should be inserted.
///
/// @return The current worst distance metric (squared).
  auto metric() const -> real_t { return worst_metric; }

/// @brief Get an iterator to the beginning of the stored result buffer.
///
/// @return Random-access iterator to the first stored result.
  auto begin() const -> RandomIt { return out; }

/// @brief Get an iterator to one past the last stored result.
///
/// @return Random-access iterator past the end of the current result set.
  auto end() const -> RandomIt { return out + count; }

/// @brief Get an iterator to the full capacity boundary.
///
/// This points to the end of the preallocated result buffer, not just the current subset.
///
/// @return Random-access iterator at the logical capacity (`out + k`).
  auto capacity() const -> RandomIt { return out + k; }

/// @brief Return the number of currently stored results.
///
/// @return The number of nearest neighbors currently held.
  auto size() const -> std::size_t { return count; }

/// @brief Check whether the container holds any results.
///
/// @return `true` if no results are stored; otherwise `false`.
  auto empty() const -> bool { return count == 0; }

private:
  RandomIt out;
  std::size_t k;
  std::size_t count;
  real_t worst_metric;
};


/// @brief Construct a `tree_knn` container with automatic type deduction.
///
/// @param iterator The output iterator pointing to a user-provided buffer.
/// @param k The number of nearest neighbors to retain.
/// @return A `tree_knn<RandomIt>` instance.
template <typename RandomIt>
auto make_tree_knn(RandomIt iterator, std::size_t k) {
  return tree_knn<RandomIt>{iterator, k};
}


/// @brief Construct a radius-limited `tree_knn` container with automatic type deduction.
///
/// @param iterator The output iterator pointing to a user-provided buffer.
/// @param k The number of nearest neighbors to retain.
/// @param radius The query radius. Only neighbors within this distance are considered.
/// @return A `tree_knn<RandomIt>` instance.
template <typename RandomIt, typename RealT>
auto make_tree_knn(RandomIt iterator, std::size_t k, RealT radius) {
  return tree_knn<RandomIt>{iterator, k, radius};
}
} // namespace tf
