/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */

#pragma once
#include "../inject_normal.hpp"
#include "../plane.hpp"
#include <utility>

namespace tf::implementation {
template <typename T, std::size_t Dims, typename Base>
struct inject_full_plane_t : Base {
  /**
   * @brief Constructs an instance by copying the plane and the base.
   *
   * @param _plane The plane to inject.
   * @param base   The base object to extend.
   */
  inject_full_plane_t(const tf::plane<T, Dims> &_plane, const Base &base)
      : Base{base}, _plane{_plane} {}

  /**
   * @brief Constructs an instance by moving the plane and the base.
   *
   * @param _plane The plane to inject (moved).
   * @param base   The base object to extend (moved).
   */
  inject_full_plane_t(tf::plane<T, Dims> &&_plane, Base &&base)
      : Base{std::move(base)}, _plane{std::move(_plane)} {}

  /**
   * @brief Returns a const reference to the injected plane.
   */
  auto plane() const -> const tf::plane<T, Dims> & { return _plane; }

  /**
   * @brief Returns a const reference to the normal vector of the injected
   * plane.
   */
  auto normal() const -> const unit_vector<T, Dims> & { return _plane.normal; }

private:
  tf::plane<T, Dims> _plane;
};

template <typename T, std::size_t Dims, typename Base>
struct inject_plane_to_normal_t : Base {
  /**
   * @brief Constructs an instance by copying the plane and the base.
   *
   * @param _plane The plane to inject.
   * @param base   The base object to extend.
   */
  inject_plane_to_normal_t(const tf::plane<T, Dims> &_plane, const Base &base)
      : Base{base}, _d{_plane.d} {}

  /**
   * @brief Constructs an instance by moving the plane and the base.
   *
   * @param _plane The plane to inject (moved).
   * @param base   The base object to extend (moved).
   */
  inject_plane_to_normal_t(tf::plane<T, Dims> &&_plane, Base &&base)
      : Base{std::move(base)}, _d{std::move(_plane.d)} {}
  /**
   * @brief Constructs an instance by copying the plane and the base.
   *
   * @param _plane The plane to inject.
   * @param base   The base object to extend.
   */
  inject_plane_to_normal_t(T _d, const Base &base) : Base{base}, _d{_d} {}

  /**
   * @brief Constructs an instance by moving the plane and the base.
   *
   * @param _plane The plane to inject (moved).
   * @param base   The base object to extend (moved).
   */
  inject_plane_to_normal_t(T _d, Base &&base) : Base{std::move(base)}, _d{_d} {}

  /**
   * @brief Returns a const reference to the injected plane.
   */
  auto plane() const -> tf::plane<T, Dims> {
    return tf::make_plane(Base::normal(), _d);
  }

  /**
   * @brief Returns a const reference to the normal vector of the injected
   * plane.
   */
  auto normal() const -> const unit_vector<T, Dims> & { return Base::normal(); }

private:
  T _d;
};

template <typename T, std::size_t Dims, typename Base>
struct inject_plane_t
    : std::conditional_t<tf::has_injected_normal<Base>,
                         inject_plane_to_normal_t<T, Dims, Base>,
                         inject_full_plane_t<T, Dims, Base>> {
private:
  using base_t = std::conditional_t<tf::has_injected_normal<Base>,
                                    inject_plane_to_normal_t<T, Dims, Base>,
                                    inject_full_plane_t<T, Dims, Base>>;

public:
  using base_t::base_t;
};

} // namespace tf::implementation
