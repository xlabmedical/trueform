/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./identity_frame.hpp"
#include "./tree.hpp"

namespace tf {

template <typename Index, typename RealT, std::size_t Dims, typename Policy>
class model : public Policy {
public:
  using real_t = RealT;
  using index_t = Index;
  using transformation_t = identity_transformation_t<real_t>;
  using frame_t = identity_frame_t<real_t>;

  model(const tf::tree<Index, RealT, Dims> &_tree, const Policy &policy)
      : Policy{policy}, _tree{_tree} {}

  auto tree() const -> const tf::tree<Index, RealT, Dims> & { return _tree; }

  auto transformation() const -> const tf::identity_transformation_t<RealT> & {
    return identity_transformation<RealT>;
  }

  auto inverse_transformation() const
      -> const tf::identity_transformation_t<RealT> & {
    return identity_transformation<RealT>;
  }

  auto frame() const -> const tf::identity_frame_t<RealT> & {
    return identity_frame<RealT>;
  }

private:
  const tf::tree<Index, RealT, Dims> &_tree;
};
} // namespace tf
