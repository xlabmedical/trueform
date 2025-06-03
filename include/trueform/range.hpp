/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include "./static_size.hpp"
#include <array>
#include <iterator>

namespace tf {
template <typename Iterator, std::size_t N> class range {
public:
  using value_type = typename std::iterator_traits<Iterator>::value_type;
  using reference = typename std::iterator_traits<Iterator>::reference;
  using pointer = typename std::iterator_traits<Iterator>::pointer;
  using iterator = Iterator;
  using const_iterator = Iterator;
  using size_type = std::size_t;

  range() = default;

  explicit range(Iterator it) : _begin(it) {}
  range(Iterator it, Iterator) : _begin(it) {}

  auto begin() const -> Iterator { return _begin; }
  auto end() const -> Iterator { return _begin + N; }

  constexpr auto size() const -> size_type { return N; }
  auto empty() const -> bool { return N == 0; }

  auto front() const -> reference { return *_begin; }
  auto back() const -> reference { return *(_begin + N - 1); }
  auto operator[](size_type i) const -> reference { return *(_begin + i); }

private:
  Iterator _begin = {};
};

template <typename Iterator, std::size_t N>
struct static_size<range<Iterator, N>>
    : std::integral_constant<std::size_t, N> {};

template <std::size_t I, typename Iterator_, std::size_t N_>
auto get(const range<Iterator_, N_> &s) -> decltype(auto) {
  static_assert(N_ != tf::dynamic_size, "Dynamic size does not support get.");
  static_assert(I < N_, "Index out of bounds");
  return s[I];
}
} // namespace tf

namespace std {
template <typename Iterator, std::size_t N>
struct tuple_size<tf::range<Iterator, N>>
    : std::integral_constant<std::size_t, N> {};

template <std::size_t I, typename Iterator, std::size_t N>
struct tuple_element<I, tf::range<Iterator, N>> {
  using type = typename std::iterator_traits<Iterator>::reference;
};

template <typename Iterator>
struct tuple_size<tf::range<Iterator, tf::dynamic_size>>;

template <std::size_t I, typename Iterator>
struct tuple_element<I, tf::range<Iterator, tf::dynamic_size>>;
} // namespace std

namespace tf {
template <typename Iterator> class range<Iterator, tf::dynamic_size> {
public:
  using value_type = typename std::iterator_traits<Iterator>::value_type;
  using reference = typename std::iterator_traits<Iterator>::reference;
  using pointer = typename std::iterator_traits<Iterator>::pointer;
  using iterator = Iterator;
  using const_iterator = Iterator;
  using size_type = std::size_t;

  range() = default;

  range(Iterator it, Iterator end) : _begin(it), _end(end) {}

  auto begin() const -> Iterator { return _begin; }
  auto end() const -> Iterator { return _end; }

  auto size() const -> size_type {
    return static_cast<size_type>(_end - _begin);
  }
  auto empty() const -> bool { return _begin == _end; }

  auto front() const -> reference { return *_begin; }
  auto back() const -> reference { return *(_end - 1); }
  auto operator[](size_type i) const -> reference { return *(_begin + i); }

private:
  Iterator _begin = {};
  Iterator _end = {};
};

template <typename Iterator>
auto make_range(Iterator it, std::size_t size)
    -> range<Iterator, tf::dynamic_size> {
  return range<Iterator, tf::dynamic_size>(it, it + size);
}

template <std::size_t N, typename Iterator>
auto make_range(Iterator it) -> range<Iterator, N> {
  return range<Iterator, N>(it);
}

template <typename Iterator>
auto make_range(Iterator it, Iterator end)
    -> range<Iterator, tf::dynamic_size> {
  return range<Iterator, tf::dynamic_size>(it, end);
}

template <std::size_t N, typename Iterator>
auto make_range(Iterator it, Iterator end) -> range<Iterator, N> {
  return range<Iterator, N>(it, end);
}

template <typename Container>
auto make_range(Container &c)
    -> range<decltype(std::begin(c)), tf::static_size_v<Container>> {
  return range<decltype(std::begin(c)), tf::static_size_v<Container>>(
      std::begin(c), std::end(c));
}

template <typename Container>
auto make_range(const Container &c)
    -> range<decltype(std::begin(c)), tf::static_size_v<Container>> {
  return range<decltype(std::begin(c)), tf::static_size_v<Container>>(
      std::begin(c), std::end(c));
}

} // namespace tf
