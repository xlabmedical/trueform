/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./buffer.hpp"

namespace tf {
template <typename Index> class mapping {
public:
  mapping() = default;

  auto f() -> tf::buffer<Index> & { return _f; }
  auto f() const -> const tf::buffer<Index> & { return _f; }
  auto kept_ids() -> tf::buffer<Index> & { return _inv_f; }
  auto kept_ids() const -> const tf::buffer<Index> & { return _inv_f; }

private:
  tf::buffer<Index> _f;
  tf::buffer<Index> _inv_f;
};
} // namespace tf
