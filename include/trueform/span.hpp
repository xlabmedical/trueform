/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once

#include <iterator>

namespace tf {

template <typename Iterator> class span {
public:
  using value_type = typename std::iterator_traits<Iterator>::value_type;
  using reference = typename std::iterator_traits<Iterator>::reference;
  using pointer = typename std::iterator_traits<Iterator>::pointer;
  using iterator = Iterator;
  using const_iterator = Iterator;
  using size_type = std::size_t;

  span() = default;

  span(Iterator it, size_type count) : _begin(it), _end(it + count) {}

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

// From iterator + size
template <typename Iterator>
auto make_span(Iterator it, std::size_t size) -> span<Iterator> {
  return span<Iterator>(it, size);
}

template <typename Iterator>
auto make_span(Iterator it, Iterator end) -> span<Iterator> {
  return span<Iterator>(it, end - it);
}

// From raw pointer + size
template <typename T> auto make_span(T *ptr, std::size_t size) -> span<T *> {
  return span<T *>(ptr, size);
}

// From container with .begin() and .size()
template <typename Container>
auto make_span(Container &c) -> span<decltype(std::begin(c))> {
  return span<decltype(std::begin(c))>(std::begin(c), c.size());
}

// Const container overload
template <typename Container>
auto make_span(const Container &c) -> span<decltype(std::begin(c))> {
  return span<decltype(std::begin(c))>(std::begin(c), c.size());
}

} // namespace tf
