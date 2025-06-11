/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./polygon.hpp"
#include "./projector.hpp"
#include "./static_size.hpp"
#include "./value_type.hpp"
#include "./point_like.hpp"
#include <cstddef>

namespace tf {

/**
 * @ingroup geometry
 * @brief Checks whether a point lies inside a polygon or on its edge (projected
 * to 2D).
 *
 * Uses the winding number algorithm, optionally accounting for numerical
 * precision via an epsilon tolerance when determining if the point lies exactly
 * on an edge.
 *
 * @tparam V Number of vertices in the polygon.
 * @tparam Policy Polygon storage policy.
 * @tparam Dims Dimensionality of the input point.
 * @tparam T Scalar type of the input point.
 * @tparam F Projector function type.
 * @param poly Polygon to test against.
 * @param input_pt The input point to check.
 * @param projector Function to project input and polygon to 2D.
 * @return true if the point is inside or on the polygon boundary.
 */
template <std::size_t V, typename Policy, std::size_t Dims, typename T,
          typename F>
auto contains_coplanar_point(const tf::polygon<V, Policy> &poly,
                             const point_like<Dims, T> &input_pt,
                             const tf::projector<F> &projector) -> bool {
  static_assert(tf::static_size_v<decltype(projector(poly[0]))> == 2,
                "We must project into 2D");
  using real_t = tf::common_value<T, decltype(poly[0][0])>;
  int winding_number = 0;
  auto n = poly.size();

  tf::point<real_t, 2> pt = projector(input_pt);

  auto check_edge = [&](const auto &a, const auto &b) {
    auto ab = b - a;
    auto ap = pt - a;
    auto dy = b[1] - a[1];

    if (a[1] <= pt[1]) {
      if (b[1] > pt[1] && (ab[0] * ap[1] - ap[0] * dy) > 0)
        ++winding_number;
    } else {
      if (b[1] <= pt[1] && (ab[0] * ap[1] - ap[0] * dy) < 0)
        --winding_number;
    }
  };

  tf::point<real_t, 2> first_pt = projector(poly[0]);
  tf::point<real_t, 2> prev_pt = first_pt;
  for (std::size_t i = 1; i < n; i++) {
    tf::point<real_t, 2> current_pt = projector(poly[i]);
    check_edge(prev_pt, current_pt);
    prev_pt = current_pt;
  }
  check_edge(prev_pt, first_pt);

  return winding_number != 0;
}

/// @ingroup geometry
/// @brief Overload for 2D input points without projection
/// @copydoc contains_coplanar_point

template <std::size_t V, typename Policy, typename T>
auto contains_coplanar_point(const tf::polygon<V, Policy> &poly,
                             const point_like<2, T> &input_pt) -> bool {
  static_assert(tf::static_size_v<decltype(poly[0])> == 2,
                "Points must be in 2D.");
  return contains_coplanar_point(poly, input_pt, tf::make_identity_projector());
}

/// @ingroup geometry
/// @brief Overload for N-dimensional input points using automatic projection
/// @copydoc contains_coplanar_point

template <std::size_t V, typename Policy, std::size_t Dims, typename T>
auto contains_coplanar_point(const tf::polygon<V, Policy> &poly,
                             const point_like<Dims, T> &input_pt) -> bool {
  return contains_coplanar_point(poly, input_pt,
                                 tf::make_simple_projector(poly));
}

/**
 * @ingroup geometry
 * @brief Checks whether a point lies inside or on a polygon with tolerance.
 *
 * Uses an epsilon to account for numerical precision when determining
 * if a point lies exactly on a polygon edge (in 2D after projection).
 *
 * @tparam V Number of vertices in the polygon.
 * @tparam Policy Polygon policy.
 * @tparam Dims Point dimensionality.
 * @tparam T Point scalar type.
 * @tparam F Projector type.
 * @param poly Polygon.
 * @param input_pt Input point.
 * @param epsilon Tolerance threshold.
 * @param projector Function that maps the input into 2D.
 * @return true if the point is inside or on the polygon.
 */
template <std::size_t V, typename Policy, std::size_t Dims, typename T,
          typename F>
auto contains_coplanar_point(const tf::polygon<V, Policy> &poly,
                             const point_like<Dims, T> &input_pt,
                             const tf::projector<F> &projector,
                             tf::common_value<T, decltype(poly[0][0])> epsilon)
    -> bool {
  static_assert(tf::static_size_v<decltype(projector(poly[0]))> == 2,
                "We must project into 2D");
  using real_t = tf::common_value<T, decltype(poly[0][0])>;
  int winding_number = 0;
  auto n = poly.size();

  tf::point<real_t, 2> pt = projector(input_pt);

  auto check_edge = [&](const auto &a, const auto &b) {
    // Check if point is on the edge (epsilon-based)

    auto ab = b - a;
    auto ap = pt - a;
    auto bp = pt - b;

    auto area = ab[0] * ap[1] - ab[1] * ap[0]; // Cross product
    auto dot0 = ap[0] * bp[0];
    auto dot1 = ap[1] * bp[1];

    if (std::abs(area) < epsilon && dot0 <= 0 && dot1 <= 0) {
      return true; // On edge → considered inside
    }

    auto dy = b[1] - a[1];

    if (a[1] <= pt[1]) {
      if (b[1] > pt[1] && (ab[0] * ap[1] - ap[0] * dy) > 0)
        ++winding_number;
    } else {
      if (b[1] <= pt[1] && (ab[0] * ap[1] - ap[0] * dy) < 0)
        --winding_number;
    }
    return false;
  };

  tf::point<real_t, 2> first_pt = projector(poly[0]);
  tf::point<real_t, 2> prev_pt = first_pt;
  for (std::size_t i = 1; i < n; i++) {
    tf::point<real_t, 2> current_pt = projector(poly[i]);
    if (check_edge(prev_pt, current_pt))
      return true;
    prev_pt = current_pt;
  }
  if (check_edge(prev_pt, first_pt))
    return true;

  return winding_number != 0;
}

/// @ingroup geometry
/// @brief Overload with epsilon and no projector (2D input)
/// @copydoc contains_coplanar_point

template <std::size_t V, typename Policy, typename T>
auto contains_coplanar_point(const tf::polygon<V, Policy> &poly,
                             const point_like<2, T> &input_pt,
                             tf::common_value<T, decltype(poly[0][0])> epsilon)
    -> bool {
  static_assert(tf::static_size_v<decltype(poly[0])> == 2,
                "Points must be in 2D.");
  return contains_coplanar_point(poly, input_pt, tf::make_identity_projector(),
                                 epsilon);
}

/// @ingroup geometry
/// @brief Overload with epsilon and auto projection
/// @copydoc contains_coplanar_point

template <std::size_t V, typename Policy, std::size_t Dims, typename T>
auto contains_coplanar_point(const tf::polygon<V, Policy> &poly,
                             const point_like<Dims, T> &input_pt,
                             tf::common_value<T, decltype(poly[0][0])> epsilon)
    -> bool {
  return contains_coplanar_point(poly, input_pt,
                                 tf::make_simple_projector(poly), epsilon);
}
} // namespace tf
