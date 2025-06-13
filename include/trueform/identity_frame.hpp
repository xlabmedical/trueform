/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./identity_transformation.hpp"

namespace tf {
template <typename RealT> struct identity_frame_t {

  auto transformation() const -> const tf::identity_transformation_t<RealT> & {
    return identity_transformation<RealT>;
  }

  auto inverse_transformation() const
      -> const tf::identity_transformation_t<RealT> & {
    return identity_transformation<RealT>;
  }
};


template <typename RealT>
constexpr identity_frame_t<RealT> identity_frame;
} // namespace tf
