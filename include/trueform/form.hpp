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
  form(tf::frame_view<RealT, Dims> _frame, tf::tree<Index, RealT, Dims> &_tree,
       Policy &&policy)
      : base_t{_tree, std::move(policy)}, _frame{_frame} {}

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

template <typename RealT, std::size_t Dims, typename Index, typename Policy>
auto make_form(const tf::frame_view<RealT, Dims> &_frame,
               tf::tree<Index, RealT, Dims> &_tree, Policy &&policy)
    -> form<Index, RealT, Dims, Policy> {
  return form<Index, RealT, Dims, Policy>{_frame, _tree, std::move(policy)};
}

template <typename RealT, std::size_t Dims, typename Index, typename Policy>
auto make_form(const tf::frame<RealT, Dims> &_frame,
               tf::tree<Index, RealT, Dims> &_tree, Policy &&policy)
    -> form<Index, RealT, Dims, Policy> {
  return form<Index, RealT, Dims, Policy>{tf::make_frame_view(_frame), _tree,
                                          std::move(policy)};
}

template <typename RealT, std::size_t Dims, typename Index, typename Policy>
auto make_form(tf::tree<Index, RealT, Dims> &_tree, Policy &&policy)
    -> model_form<Index, RealT, Dims, Policy> {
  return make_model_form(_tree, std::move(policy));
}

} // namespace tf
