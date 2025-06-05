/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./aabb.hpp"
#include "./vector_view.hpp"

namespace tf {


/// @ingroup geometry
/// @brief Expand an AABB to include another AABB, in-place.
///
/// Updates `aabb0` to include the bounds of `aabb1`. Modifies `aabb0` directly.
///
/// @tparam T The scalar coordinate type.
/// @tparam N The spatial dimension.
/// @param aabb0 The AABB to be expanded.
/// @param aabb1 The AABB to include.
/// @return A reference to `aabb0`.
template <typename T, std::size_t N>
auto aabb_union_inplace(aabb<T, N> &aabb0, const aabb<T, N> &aabb1)
    -> aabb<T, N> & {
  for (int i = 0; i < int(N); i++) {
    aabb0.min[i] = std::min(aabb0.min[i], aabb1.min[i]);
    aabb0.max[i] = std::max(aabb0.max[i], aabb1.max[i]);
  }
  return aabb0;
}


/// @ingroup geometry
/// @brief Compute the union of two AABBs.
///
/// Returns a new AABB that bounds both `aabb0` and `aabb1`.
/// Neither input is modified.
///
/// @tparam T The scalar coordinate type.
/// @tparam N The spatial dimension.
/// @param aabb0 The first bounding box.
/// @param aabb1 The second bounding box.
/// @return An `aabb<T, N>` containing both inputs.
template <typename T, std::size_t N>
auto aabb_union(const aabb<T, N> &aabb0, const aabb<T, N> &aabb1)
    -> aabb<T, N> {
  aabb<T, N> out = aabb0;
  aabb_union_inplace(out, aabb1);
  return out;
}


/// @ingroup geometry
/// @brief Expand an AABB to include a point, in-place.
///
/// Updates `aabb0` to include the given point `pt`.
///
/// @tparam T The scalar coordinate type.
/// @tparam N The spatial dimension.
/// @param aabb0 The AABB to be expanded.
/// @param pt The point to include.
/// @return A reference to `aabb0`.
template <typename T, std::size_t N>
auto aabb_union_inplace(aabb<T, N> &aabb0, const vector<T, N> &pt)
    -> aabb<T, N> & {
  for (int i = 0; i < int(N); i++) {
    aabb0.min[i] = std::min(aabb0.min[i], pt[i]);
    aabb0.max[i] = std::max(aabb0.max[i], pt[i]);
  }
  return aabb0;
}


/// @ingroup geometry
/// @brief Compute the union of an AABB and a point.
///
/// Returns a new AABB that includes both the input bounding box and the given point.
/// The original AABB is not modified.
///
/// @tparam T The scalar coordinate type.
/// @tparam N The spatial dimension.
/// @param aabb0 The bounding box.
/// @param pt The point to include.
/// @return An `aabb<T, N>` containing the original box and the point.
template <typename T, std::size_t N>
auto aabb_union(const aabb<T, N> &aabb0, const vector<T, N> &pt) -> aabb<T, N> {
  aabb<T, N> out = aabb0;
  aabb_union_inplace(out, pt);
  return out;
}


/// @ingroup geometry
/// @brief Expand an AABB to include a `vector_view`, in-place.
///
/// Updates `aabb0` to include the point given as a view. Useful for referencing raw or external data.
///
/// @tparam T The scalar coordinate type.
/// @tparam N The spatial dimension.
/// @param aabb0 The AABB to be expanded.
/// @param pt The point view to include.
/// @return A reference to `aabb0`.
template <typename T, std::size_t N>
auto aabb_union_inplace(aabb<T, N> &aabb0, const vector_view<T, N> &pt)
    -> aabb<T, N> & {
  for (int i = 0; i < int(N); i++) {
    aabb0.min[i] = std::min(aabb0.min[i], pt[i]);
    aabb0.max[i] = std::max(aabb0.max[i], pt[i]);
  }
  return aabb0;
}


/// @ingroup geometry
/// @brief Compute the union of an AABB and a `vector_view`.
///
/// Returns a new AABB that includes both the input bounding box and the point view.
/// The original AABB is not modified.
///
/// @tparam T The scalar coordinate type.
/// @tparam N The spatial dimension.
/// @param aabb0 The bounding box.
/// @param pt The point view to include.
/// @return An `aabb<T, N>` containing the original box and the point.
template <typename T, std::size_t N>
auto aabb_union(const aabb<T, N> &aabb0, const vector_view<T, N> &pt)
    -> aabb<T, N> {
  aabb<T, N> out = aabb0;
  aabb_union_inplace(out, pt);
  return out;
}

} // namespace tf
