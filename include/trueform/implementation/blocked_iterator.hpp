/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "../range.hpp"
#include "../static_size.hpp"
#include "./stride_iterator_api.hpp"

namespace tf::implementation::iter {
template <typename Iterator, std::size_t BlockSize> struct blocked_api_handle {
public:
  using iterator_category =
      typename std::iterator_traits<Iterator>::iterator_category;
  using reference = tf::range<Iterator, BlockSize>;
  using value_type = reference;
  using pointer = void;
  using difference_type =
      typename std::iterator_traits<Iterator>::difference_type;

  blocked_api_handle() = default;
  blocked_api_handle(Iterator iter) : iter{std::move(iter)} {}
  auto base_iter() const -> const Iterator & { return iter; }
  auto base_iter() -> Iterator & { return iter; }
  constexpr auto iterator_stride() const -> std::size_t { return BlockSize; }
  auto dereference() const -> reference {
    return tf::make_range<BlockSize>(iter);
  }

private:
  Iterator iter;
};

template <typename Iterator>
struct blocked_api_handle<Iterator, tf::dynamic_size> {
public:
  using iterator_category =
      typename std::iterator_traits<Iterator>::iterator_category;
  using reference = tf::range<Iterator, tf::dynamic_size>;
  using value_type = reference;
  using pointer = void;
  using difference_type =
      typename std::iterator_traits<Iterator>::difference_type;

  blocked_api_handle(Iterator iter, std::size_t stride)
      : iter{std::move(iter)}, stride{std::move(stride)} {}
  auto base_iter() const -> const Iterator & { return iter; }
  auto base_iter() -> Iterator & { return iter; }
  auto iterator_stride() const -> std::size_t { return stride; }
  auto dereference() const -> reference { return tf::make_range(iter, stride); }

private:
  Iterator iter;
  std::size_t stride;
};

template <typename Iterator, std::size_t BlockSize>
struct blocked_iterator : stride_api<blocked_iterator<Iterator, BlockSize>,
                                     blocked_api_handle<Iterator, BlockSize>> {
private:
  using base_t = stride_api<blocked_iterator<Iterator, BlockSize>,
                            blocked_api_handle<Iterator, BlockSize>>;
  using handle_t = blocked_api_handle<Iterator, BlockSize>;

public:
  explicit blocked_iterator(Iterator iter)
      : base_t{handle_t{std::move(iter)}} {}
};

template <typename Iterator>
struct blocked_iterator<Iterator, tf::dynamic_size>
    : stride_api<blocked_iterator<Iterator, tf::dynamic_size>,
                 blocked_api_handle<Iterator, tf::dynamic_size>> {
private:
  using base_t = stride_api<blocked_iterator<Iterator, tf::dynamic_size>,
                            blocked_api_handle<Iterator, tf::dynamic_size>>;
  using handle_t = blocked_api_handle<Iterator, tf::dynamic_size>;

public:
  blocked_iterator(Iterator iter, std::size_t block_size)
      : base_t{handle_t{std::move(iter), std::move(block_size)}} {}
};

template <typename Iterator>
auto make_blocked_iterator(Iterator iter, std::size_t block_size) {
  return blocked_iterator<Iterator, tf::dynamic_size>{std::move(iter),
                                                      std::move(block_size)};
}

template <std::size_t BlockSize, typename Iterator>
auto make_blocked_iterator(Iterator iter) {
  return blocked_iterator<Iterator, BlockSize>{std::move(iter)};
}

} // namespace tf::implementation::iter
