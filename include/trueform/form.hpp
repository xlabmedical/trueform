/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./dyn_model.hpp"
#include "./frame.hpp"
#include "./frame_view.hpp"
#include "./model.hpp"

namespace tf {

template <std::size_t Dims, typename Policy> struct form : public Policy {
  form(Policy &&policy) : Policy{std::move(policy)} {}
  form(const Policy &policy) : Policy{policy} {}
};

template <typename RealT, std::size_t Dims, typename Index, typename Policy>
auto make_form(const tf::frame_view<RealT, Dims> &_frame,
               tf::tree<Index, RealT, Dims> &_tree, Policy &&policy)
    -> form<Dims, dyn_model<Index, RealT, Dims, std::decay_t<Policy>>> {
  return form<Dims, dyn_model<Index, RealT, Dims, std::decay_t<Policy>>>{
      dyn_model<Index, RealT, Dims, std::decay_t<Policy>>{
          _frame, _tree, static_cast<Policy &&>(policy)}};
}

template <typename RealT, std::size_t Dims, typename Index, typename Policy>
auto make_form(const tf::frame<RealT, Dims> &_frame,
               tf::tree<Index, RealT, Dims> &_tree, Policy &&policy)
    -> form<Dims, dyn_model<Index, RealT, Dims, std::decay_t<Policy>>> {
  return form<Dims, dyn_model<Index, RealT, Dims, std::decay_t<Policy>>>{
      dyn_model<Index, RealT, Dims, std::decay_t<Policy>>{
          tf::make_frame_view(_frame), _tree, static_cast<Policy &&>(policy)}};
}

template <typename RealT, std::size_t Dims, typename Index, typename Policy>
auto make_form(tf::tree<Index, RealT, Dims> &_tree, Policy &&policy)
    -> form<Dims, model<Index, RealT, Dims, std::decay_t<Policy>>> {
  return form<Dims, model<Index, RealT, Dims, std::decay_t<Policy>>>{
      model<Index, RealT, Dims, std::decay_t<Policy>>{
          _tree, static_cast<Policy &&>(policy)}};
}
} // namespace tf
