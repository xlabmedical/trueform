/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./vector.hpp"
#include <array>

namespace tf {

/// @ingroup geometry
/// @brief Represents an affine transformation matrix.
///
/// A `transformation` stores a D×(D+1) matrix for applying affine transformations
/// to D-dimensional points and vectors. It supports both point and vector transformations,
/// distinguishing between them by including or excluding the translation term.
/// 
/// Typically created using utility functions such as:
/// - @ref make_identity_transformation()
/// - @ref make_transformation_from_translation()
///
/// @tparam T The scalar type used for coordinates (e.g., float or double).
/// @tparam Dims The dimensionality of the space (typically 2 or 3).
template <typename T, std::size_t Dims> class transformation {
public:
  /// @brief Default constructor. Initializes an uninitialized transformation.
  transformation() = default;

  /// @brief Construct from a nested array of transformation coefficients.
  /// @param t A D×(D+1) nested array representing the affine transform.
  transformation(const std::array<std::array<T, 4>, Dims> &t) : _transform{t} {}

  /// @brief Accessor for transformation matrix elements (const).
  /// @param i Row index (0 ≤ i < Dims)
  /// @param j Column index (0 ≤ j ≤ Dims)
  /// @return A const reference to the (i, j)th element.
  auto operator()(std::size_t i, std::size_t j) const -> const T & {
    return _transform[i][j];
  }

  /// @brief Accessor for transformation matrix elements (mutable).
  /// @param i Row index (0 ≤ i < Dims)
  /// @param j Column index (0 ≤ j ≤ Dims)
  /// @return A reference to the (i, j)th element.
  auto operator()(std::size_t i, std::size_t j) -> T & {
    return _transform[i][j];
  }

  /// @brief Transform a point (with translation).
  /// @param point The input point to transform.
  /// @param out The output point, overwritten with the result.
  template <typename Point0, typename Point1>
  auto transform_point(const Point0 &point, Point1 &&out) const {
    auto size = Dims;
    for (decltype(size) i = 0; i < size; ++i) {
      out[i] = _transform[i][size];
      for (decltype(size) j = 0; j < size; ++j) {
        out[i] += point[j] * _transform[i][j];
      }
    }
  }

  /// @brief Transform a point and return the result.
  /// @param point The input point to transform.
  /// @return A new point of type `tf::vector<T, Dims>` representing the result.
  template <typename Point>
  auto transform_point(const Point &point) const {
    tf::vector<decltype(point[0] * T()), Dims> out;
    transform_point(point, out);
    return out;
  }

  /// @brief Transform a vector (ignores translation).
  /// @param point The input vector to transform.
  /// @param out The output vector, overwritten with the result.
  template <typename Point0, typename Point1>
  auto transform_vector(const Point0 &point, Point1 &&out) const {
    auto size = Dims;
    for (decltype(size) i = 0; i < size; ++i) {
      out[i] = 0;
      for (decltype(size) j = 0; j < size; ++j) {
        out[i] += point[j] * _transform[i][j];
      }
    }
  }

  /// @brief Transform a vector and return the result.
  /// @param point The input vector to transform.
  /// @return A new vector of the same type as the input.
  template <typename Point>
  auto transform_vector(const Point &point) const {
    decltype(point - point) out;
    transform_vector(point, out);
    return out;
  }

private:
  std::array<std::array<T, Dims + 1>, Dims> _transform;
};

/// @ingroup geometry
/// @brief Create an identity affine transformation.
/// 
/// Initializes a matrix where the top-left D×D block is the identity and
/// the translation column is zero.
///
/// @tparam T Scalar type.
/// @tparam Dims Dimensionality of the transformation.
/// @return A new identity @ref tf::transformation.
template <typename T, std::size_t Dims>
auto make_identity_transformation() {
  transformation<T, Dims> out;
  for (std::size_t i = 0; i < Dims; ++i)
    for (std::size_t j = 0; j < Dims + 1; ++j)
      out(i, j) = i == j;
  return out;
}

/// @ingroup geometry
/// @brief Create a pure translation transformation.
///
/// Constructs an affine transformation that applies only translation,
/// using the provided translation vector.
///
/// @tparam T Scalar type.
/// @tparam Dims Dimensionality of the transformation.
/// @param translation A vector representing the translation offset.
/// @return A @ref tf::transformation that performs only translation.
template <typename T, std::size_t Dims>
auto make_transformation_from_translation(
    const tf::vector<T, Dims> &translation) {
  auto out = make_identity_transformation<T, Dims>();
  for (std::size_t i = 0; i < Dims; ++i)
    out(i, Dims) = translation[i];
  return out;
}
} // namespace tf
