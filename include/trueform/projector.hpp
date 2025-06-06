/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./inject_normal.hpp"
#include "./inject_plane.hpp"
#include "./polygon.hpp"
#include "./value_type.hpp"
#include "./vector.hpp"
#include "./vector_like.hpp"

namespace tf {

/// @ingroup geometry
/// @brief A wrapper around a callable object used to project a point into a
/// lower-dimensional space.
///
/// `projector<F>` wraps a callable object `F` and provides a projection
/// operator for `vector_like` inputs. This allows geometric structures like
/// planes or polygons to define custom projection behavior for operations such
/// as containment or distance calculation.
///
/// @tparam F A callable type implementing `operator()(vector_like)` returning a
/// projected point.
template <typename F> struct projector : private F {
  /// @brief Constructs a projector from a given function object.
  /// @param f The projection function or functor to wrap.
  projector(F f) : F{std::move(f)} {}

  /// @brief Applies the projector to a vector-like object.
  /// @tparam Dims The number of dimensions of the input vector.
  /// @tparam T The vector data type.
  /// @param pt The point to project.
  /// @return The result of applying the wrapped projection function.
  template <std::size_t Dims, typename T>
  auto operator()(const vector_like<Dims, T> &pt) const -> decltype(auto) {
    return static_cast<const F &>(*this)(pt);
  }
};

/// @ingroup geometry
/// @brief Creates a `projector` from a given callable object.
///
/// Convenience factory for wrapping a lambda or functor into a `projector`.
///
/// @tparam F The callable type.
/// @param f The callable to wrap.
/// @return A `projector` wrapping the provided callable.
template <typename F> auto make_projector(F &&f) {
  return projector<std::decay_t<F>>{static_cast<F &&>(f)};
}

/// @ingroup geometry
/// @brief Creates an identity projector that returns the input unchanged.
///
/// Useful when no projection is needed but an interface expects a projector.
///
/// @return A projector that returns its input unchanged.
inline auto make_identity_projector() {
  return tf::make_projector([](const auto &x) -> const auto & { return x; });
}

/// @ingroup geometry
/// @brief Creates a simple 2D projection from a 3D normal vector.
///
/// This function chooses the two coordinate axes most orthogonal to the normal,
/// and returns a projector that maps a 3D vector to a 2D vector in that plane.
///
/// The largest component of the normal is dropped to avoid distortion.
///
/// @tparam T The scalar type of the normal.
/// @param normal The normal vector (assumed normalized).
/// @return A projector mapping 3D points to 2D using orthogonal axes.
template <typename T>
auto make_simple_projector(const vector_like<3, T> &normal) {
  std::array<int, 2> ids{0, 1};
  auto max = std::abs(normal[0]);
  int k = 0;
  for (int i = 1; i < 3; i++) {
    auto tmp = std::abs(normal[i]);
    if (tmp > max) {
      k = i;
      max = tmp;
    }
  }
  if (k == 1) {
    ids[0] = 2;
    ids[1] = 0;
  } else if (k == 0) {
    ids[0] = 1;
    ids[1] = 2;
  }

  if (normal[k] < 0)
    std::swap(ids[0], ids[1]);

  return make_projector([x = ids[0], y = ids[1]](const auto &pt) {
    return tf::vector<tf::value_type<T>, 2>{{pt[x], pt[y]}};
  });
}

/// @ingroup geometry
/// @brief Creates a simple projector from an object with an injected normal.
///
/// Overload for types that have a `.normal` field, such as `inject_normal_t`.
///
/// @tparam T Scalar type of the normal.
/// @tparam Dims Dimensionality (should be 3).
/// @tparam Base The base type that contains the normal.
/// @param poly The object with an injected normal vector.
/// @return A projector that projects points into the orthogonal 2D plane.
template <typename T, std::size_t Dims, typename Base>
auto make_simple_projector(const inject_normal_t<T, Dims, Base> &poly) {
  return make_simple_projector(poly.normal);
}

/// @ingroup geometry
/// @brief Creates a simple projector from an object with an injected plane.
///
/// Overload for types that have a `.normal` field, such as `inject_plane_t`.
///
/// @tparam T Scalar type of the normal.
/// @tparam Dims Dimensionality (should be 3).
/// @tparam Base The base type that contains the plane.
/// @param poly The object with an injected plane (must define `.normal`).
/// @return A projector that projects points into the orthogonal 2D plane.
template <typename T, std::size_t Dims, typename Base>
auto make_simple_projector(const inject_plane_t<T, Dims, Base> &poly) {
  return make_simple_projector(poly.normal);
}

/// @ingroup geometry
/// @brief Creates a projector for a polygon in 3D space.
///
/// Uses the cross product of the first three vertices to infer a normal,
/// and generates a 2D projection plane orthogonal to it.
///
/// @tparam V Number of vertices in the polygon.
/// @tparam T Polygon storage policy.
/// @param poly A 3D polygon.
/// @return A projector that maps 3D points in the polygon to 2D.
template <std::size_t V, typename T>
auto make_simple_projector(const polygon<V, T> &poly) {
  static_assert(tf::static_size_v<decltype(poly[0][0])> == 3,
                "Only makes sense when dimension is 3");
  make_simple_projector(tf::cross(poly[2] - poly[0], poly[0] - poly[1]));
}

} // namespace tf
