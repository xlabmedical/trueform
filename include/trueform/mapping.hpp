/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./buffer.hpp"

namespace tf {
template <typename Range0, typename Range1> class mapping {
public:
  mapping() = default;
  mapping(const Range0 &_f, const Range1 &_inv_f) : _f{_f}, _inv_f{_inv_f} {}
  mapping(Range0 &&_f, Range1 &&_inv_f)
      : _f{std::move(_f)}, _inv_f{std::move(_inv_f)} {}

  auto f() -> Range0 & { return _f; }
  auto f() const -> const Range0 & { return _f; }
  auto kept_ids() -> Range1 & { return _inv_f; }
  auto kept_ids() const -> const Range1 & { return _inv_f; }

private:
  Range0 _f;
  Range1 _inv_f;
};

template <typename Range0, typename Range1>
auto make_mapping(Range0 &&_f, Range1 &&_kept_ids) {
  return mapping<std::decay_t<Range0>, std::decay_t<Range1>>{
      static_cast<Range0 &&>(_f), static_cast<Range1 &&>(_kept_ids)};
}

template <typename Index>
class mapping_val : public mapping<tf::buffer<Index>, tf::buffer<Index>> {};
} // namespace tf
