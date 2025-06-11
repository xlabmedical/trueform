/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./frame.hpp"
#include "./frame_view.hpp"
#include "./model_form.hpp"

namespace tf {

template <typename Index, typename RealT, std::size_t Dims, typename Policy>
class form : public model_form<Index, RealT, Dims, Policy> {
  using base_t = model_form<Index, RealT, Dims, Policy>;

public:
  form(tf::tree<Index, RealT, Dims> &_tree, tf::frame_view<RealT, Dims> _frame,
       Policy &&policy)
      : base_t{_tree, std::move(policy)}, _frame{_frame} {}

  auto transformation() const -> const tf::transformation<RealT, Dims> & {
    return _frame.transformation();
  }

  auto inverse_transform() const -> const tf::transformation<RealT, Dims> & {
    return _frame.inverse_transform();
  }

  auto frame() const -> const tf::frame_view<RealT, Dims> & { return _frame; }

private:
  tf::frame_view<RealT, Dims> _frame;
};

template <typename Index, typename RealT, std::size_t Dims, typename Policy>
auto make_form(tf::tree<Index, RealT, Dims> &_tree,
               const tf::frame_view<RealT, Dims> &_frame, Policy &&policy)
    -> form<Index, RealT, Dims, Policy> {
  return form<Index, RealT, Dims, Policy>{_tree, _frame, std::move(policy)};
}

template <typename Index, typename RealT, std::size_t Dims, typename Policy>
auto make_form(tf::tree<Index, RealT, Dims> &_tree,
               const tf::frame<RealT, Dims> &_frame, Policy &&policy)
    -> form<Index, RealT, Dims, Policy> {
  return form<Index, RealT, Dims, Policy>{_tree, tf::make_frame_view(_frame),
                                          std::move(policy)};
}

template <typename Index, typename RealT, std::size_t Dims, typename Policy>
auto make_form(tf::tree<Index, RealT, Dims> &_tree, Policy &&policy)
    -> model_form<Index, RealT, Dims, Policy> {
  return make_model_form(_tree, std::move(policy));
}

} // namespace tf
