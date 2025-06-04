/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./buffer.hpp"

namespace tf {

/// @brief A utility class for representing a remapping of elements, such as point or polygon IDs.
///
/// `mapping` is used to remap a set of indices (e.g., after a filtering or compression operation).
/// The `f()` accessor returns the mapping function (typically an array of new indices),
/// while `kept_ids()` returns the original indices that were retained.
///
/// This is typically used in scenarios where some elements are removed, and the remaining ones
/// must be updated to reflect their new positions in memory.
///
/// Use @ref tf::make_mapping factory to create an instance.
///
/// @tparam Range0 Type representing the mapping function.
/// @tparam Range1 Type representing the list of kept IDs.
template <typename Range0, typename Range1>
class mapping {
public:
  /// @brief Default constructor.
  mapping() = default;

  /// @brief Constructs the mapping from two ranges.
  /// @param _f The forward mapping range (e.g., new index for each original ID).
  /// @param _inv_f The list of original IDs that were retained.
  mapping(const Range0 &_f, const Range1 &_inv_f) : _f{_f}, _inv_f{_inv_f} {}

  /// @brief Move constructor from two ranges.
  /// @param _f The forward mapping range.
  /// @param _inv_f The list of retained IDs.
  mapping(Range0 &&_f, Range1 &&_inv_f)
      : _f{std::move(_f)}, _inv_f{std::move(_inv_f)} {}

  /// @brief Returns a mutable reference to the mapping function.
  auto f() -> Range0 & { return _f; }

  /// @brief Returns a const reference to the mapping function.
  auto f() const -> const Range0 & { return _f; }

  /// @brief Returns a mutable reference to the list of kept IDs.
  auto kept_ids() -> Range1 & { return _inv_f; }

  /// @brief Returns a const reference to the list of kept IDs.
  auto kept_ids() const -> const Range1 & { return _inv_f; }

private:
  Range0 _f;
  Range1 _inv_f;
};

/// @brief Creates a `mapping` object by inferring types from provided ranges.
///
/// @param _f The mapping function range.
/// @param _kept_ids The list of original retained IDs.
/// @return A `mapping` object with the deduced types.
template <typename Range0, typename Range1>
auto make_mapping(Range0 &&_f, Range1 &&_kept_ids) {
  return mapping<std::decay_t<Range0>, std::decay_t<Range1>>{
      static_cast<Range0 &&>(_f), static_cast<Range1 &&>(_kept_ids)};
}

/// @brief A convenient alias for a mapping with an owning @ref tf::buffer.
template <typename Index>
class mapping_val : public mapping<tf::buffer<Index>, tf::buffer<Index>> {};

} // namespace tf
