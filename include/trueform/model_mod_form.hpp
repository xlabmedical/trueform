/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./mod_tree.hpp"

namespace tf {

template <typename Index, typename RealT, std::size_t Dims, typename Policy>
class model_mod_form : public Policy {
public:
  model_mod_form(const tf::mod_tree<Index, RealT, Dims> &_tree, Policy &&policy)
      : Policy{std::move(policy)}, _tree{_tree} {}

  auto tree() const -> const tf::mod_tree<Index, RealT, Dims> & {
    return _tree;
  }

private:
  const tf::mod_tree<Index, RealT, Dims> &_tree;
};

template <typename Index, typename RealT, std::size_t Dims, typename Policy>
auto make_model_mod_form(const tf::mod_tree<Index, RealT, Dims> &_tree,
                         Policy &&policy)
    -> model_mod_form<Index, RealT, Dims, Policy> {
  return model_mod_form<Index, RealT, Dims, Policy>{_tree, std::move(policy)};
}

template <typename Index, typename RealT, std::size_t Dims, typename Policy>
auto make_model_form(const tf::mod_tree<Index, RealT, Dims> &_tree,
                     Policy &&policy)
    -> model_mod_form<Index, RealT, Dims, Policy> {
  return model_mod_form<Index, RealT, Dims, Policy>{_tree, std::move(policy)};
}
} // namespace tf
