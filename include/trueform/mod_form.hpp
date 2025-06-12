/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./form.hpp"
#include "./frame.hpp"
#include "./frame_view.hpp"
#include "./model_mod_form.hpp"

namespace tf {

template <typename Index, typename RealT, std::size_t Dims, typename Policy>
class mod_form : public model_mod_form<Index, RealT, Dims, Policy> {
  using base_t = model_mod_form<Index, RealT, Dims, Policy>;

public:
  mod_form(tf::frame_view<RealT, Dims> _frame,
           const tf::mod_tree<Index, RealT, Dims> &_tree, Policy &&policy)
      : base_t{_tree, std::move(policy)}, _frame{_frame} {}

  auto main_form() const -> form<Index, RealT, Dims, Policy> {
    return tf::make_form(_frame, base_t::tree().main_tree(),
                         static_cast<const Policy &>(*this));
  }

  auto delta_form() const -> form<Index, RealT, Dims, Policy> {
    return tf::make_form(_frame, base_t::tree().delta_tree(),
                         static_cast<const Policy &>(*this));
  }

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
auto make_mod_form(const tf::frame_view<RealT, Dims> &_frame,
                   const tf::mod_tree<Index, RealT, Dims> &_tree,
                   Policy &&policy) -> mod_form<Index, RealT, Dims, Policy> {
  return mod_form<Index, RealT, Dims, Policy>{_frame, _tree, std::move(policy)};
}

template <typename RealT, std::size_t Dims, typename Index, typename Policy>
auto make_mod_form(const tf::frame<RealT, Dims> &_frame,
                   const tf::mod_tree<Index, RealT, Dims> &_tree,
                   Policy &&policy) -> mod_form<Index, RealT, Dims, Policy> {
  return mod_form<Index, RealT, Dims, Policy>{tf::make_frame_view(_frame),
                                              _tree, std::move(policy)};
}

template <typename RealT, std::size_t Dims, typename Index, typename Policy>
auto make_mod_form(const tf::mod_tree<Index, RealT, Dims> &_tree,
                   Policy &&policy)
    -> model_mod_form<Index, RealT, Dims, Policy> {
  return make_model_mod_form(_tree, std::move(policy));
}

template <typename RealT, std::size_t Dims, typename Index, typename Policy>
auto make_form(const tf::frame_view<RealT, Dims> &_frame,
               const tf::mod_tree<Index, RealT, Dims> &_tree, Policy &&policy)
    -> mod_form<Index, RealT, Dims, Policy> {
  return mod_form<Index, RealT, Dims, Policy>{_frame, _tree, std::move(policy)};
}

template <typename RealT, std::size_t Dims, typename Index, typename Policy>
auto make_form(const tf::frame<RealT, Dims> &_frame,
               const tf::mod_tree<Index, RealT, Dims> &_tree, Policy &&policy)
    -> mod_form<Index, RealT, Dims, Policy> {
  return mod_form<Index, RealT, Dims, Policy>{tf::make_frame_view(_frame),
                                              _tree, std::move(policy)};
}

template <typename RealT, std::size_t Dims, typename Index, typename Policy>
auto make_form(const tf::mod_tree<Index, RealT, Dims> &_tree, Policy &&policy)
    -> model_mod_form<Index, RealT, Dims, Policy> {
  return make_model_mod_form(_tree, std::move(policy));
}
} // namespace tf
