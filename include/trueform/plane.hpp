/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./dot.hpp"
#include "./normal.hpp"
#include "./unit_vector_like.hpp"

namespace tf {

/// @ingroup geometry
/// @brief A geometric plane in N-dimensional space.
///
/// The plane is represented in Hessian normal form: a unit normal vector `n`
/// and a scalar offset `d`, such that any point `p` lies on the plane if:
///
/// @code
/// dot(n, p) + d == 0
/// @endcode
///
/// @tparam T Scalar type (e.g., float, double).
/// @tparam Dims Number of spatial dimensions.
template <typename T, std::size_t Dims> struct plane {
  tf::unit_vector<T, Dims> normal; ///< Unit-length normal vector of the plane.
  T d;                             ///< Signed offset from the origin.
};

/// @ingroup geometry
/// @brief Constructs a plane from a normal and offset.
///
/// Assumes the given normal is already normalized. This is a low-level overload
/// useful when the offset `d` is precomputed or known.
///
/// @tparam T Scalar type.
/// @tparam N Number of dimensions.
/// @param normal A unit-length normal vector.
/// @param d Signed offset from the origin.
/// @return A `plane<T, N>` representing the given plane.
template <std::size_t N, typename T>
auto make_plane(const unit_vector_like<N, T> &normal,
                typename T::value_type d) {
  return plane<typename T::value_type, N>{normal, d};
}

/// @ingroup geometry
/// @brief Constructs a plane from a normal vector and a point on the plane.
///
/// Computes the plane offset from the point and the unit normal vector:
/// `d = -dot(normal, point)`.
///
/// @tparam T0 Scalar type of the unit normal.
/// @tparam N Number of dimensions.
/// @tparam T1 A vector-like type convertible to a point.
/// @param normal A unit-length normal vector.
/// @param point A point on the plane.
/// @return A `plane<T0, N>` that passes through the given point.
template <std::size_t N, typename T0, typename T1>
auto make_plane(const unit_vector_like<N, T0> &normal,
                const vector_like<N, T1> &point) {
  auto val = -tf::dot(normal, point);
  return plane<decltype(val), N>{normal, val};
}

/// @ingroup geometry
/// @brief Constructs a plane from three points.
///
/// Computes the normal vector via the cross product (or generalization in N-D),
/// then calculates the offset so the plane passes through the first point.
///
/// @tparam Dims Number of dimensions.
/// @tparam T0, T1, T2 Vector-like input types for the three points.
/// @param pt0 First point (used to compute the offset).
/// @param pt1 Second point (used for the normal).
/// @param pt2 Third point (used for the normal).
/// @return A `plane` defined by the three points.
template <std::size_t Dims, typename T0, typename T1, typename T2>
auto make_plane(const vector_like<Dims, T0> &pt0,
                const vector_like<Dims, T1> &pt1,
                const vector_like<Dims, T2> &pt2) {
  auto normal = tf::normal(pt0, pt1, pt2);
  auto val = -tf::dot(normal, pt0);
  return tf::plane<decltype(val), Dims>{normal, val};
}
} // namespace tf
