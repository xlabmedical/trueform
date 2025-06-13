/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./frame_view.hpp"
#include "./model.hpp"

namespace tf {

template <typename Index, typename RealT, std::size_t Dims, typename Policy>
class dyn_model : public model<Index, RealT, Dims, Policy> {
  using base_t = model<Index, RealT, Dims, Policy>;

public:
  using transformation_t = tf::transformation<RealT, Dims>;
  using frame_t = tf::frame_view<RealT, Dims>;

  dyn_model(tf::frame_view<RealT, Dims> _frame,
            tf::tree<Index, RealT, Dims> &_tree, const Policy &policy)
      : base_t{_tree, policy}, _frame{_frame} {}

  auto transformation() const -> const tf::transformation<RealT, Dims> & {
    return _frame.transformation();
  }

  auto inverse_transformation() const
      -> const tf::transformation<RealT, Dims> & {
    return _frame.inverse_transformation();
  }

  auto frame() const -> const tf::frame_view<RealT, Dims> & { return _frame; }

private:
  tf::frame_view<RealT, Dims> _frame;
};
} // namespace tf
