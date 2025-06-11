/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./transformation.hpp"
#include "./tree.hpp"

namespace tf {

template <typename Index, typename RealT, std::size_t Dims, typename Policy>
class model_form : public Policy {
public:
  model_form(tf::tree<Index, RealT, Dims> &_tree, Policy &&policy)
      : Policy{std::move(policy)}, _tree{_tree} {}

  auto tree() -> tf::tree<Index, RealT, Dims> & { return _tree; }

  auto tree() const -> const tf::tree<Index, RealT, Dims> & { return _tree; }

private:
  tf::tree<Index, RealT, Dims> &_tree;
};

template <typename Index, typename RealT, std::size_t Dims, typename Policy>
class form : public model_form<Index, RealT, Dims, Policy> {
  using base_t = model_form<Index, RealT, Dims, Policy>;

public:
  form(tf::tree<Index, RealT, Dims> &_tree,
       tf::transformation<RealT, Dims> &_transformation, Policy &&policy)
      : base_t{_tree, std::move(policy)}, _transformation{_transformation} {}

  auto transformation() -> tf::transformation<RealT, Dims> & {
    return _transformation;
  }

  auto transformation() const -> const tf::transformation<RealT, Dims> & {
    return _transformation;
  }

private:
  tf::transformation<RealT, Dims> &_transformation;
};

template <typename Index, typename RealT, std::size_t Dims, typename Policy>
auto make_form(tf::tree<Index, RealT, Dims> &_tree,
               tf::transformation<RealT, Dims> &_transformation,
               Policy &&policy) -> form<Index, RealT, Dims, Policy> {
  return form<Index, RealT, Dims, Policy>{_tree, _transformation,
                                          std::move(policy)};
}

template <typename Index, typename RealT, std::size_t Dims, typename Policy>
auto make_form(tf::tree<Index, RealT, Dims> &_tree, Policy &&policy)
    -> model_form<Index, RealT, Dims, Policy> {
  return model_form<Index, RealT, Dims, Policy>{_tree, std::move(policy)};
}

} // namespace tf
