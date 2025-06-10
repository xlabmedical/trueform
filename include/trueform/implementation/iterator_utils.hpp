#pragma once

#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>
namespace tf::implementation::iter {

template <typename Iterator>
struct iterator_traits : std::iterator_traits<Iterator> {};

template <typename Iterator0, typename Iterator1>
struct iterator_traits<std::pair<Iterator0, Iterator1>> {
  using iterator_category = std::common_type_t<
      typename std::iterator_traits<Iterator0>::iterator_category,
      typename std::iterator_traits<Iterator1>::iterator_category>;
  using difference_type = std::common_type_t<
      typename std::iterator_traits<Iterator0>::difference_type,
      typename std::iterator_traits<Iterator1>::difference_type>;
};

template <typename Iterator> auto add(Iterator &it, std::size_t n) { it += n; }

template <typename Iterator0, typename Iterator1>
auto add(std::pair<Iterator0, Iterator1> &it, std::size_t n) {
  it.first += n;
  it.second += n;
}

template <typename Iterator> auto subtract(Iterator &it, std::size_t n) {
  it -= n;
}

template <typename Iterator0, typename Iterator1>
auto subtract(std::pair<Iterator0, Iterator1> &it, std::size_t n) {
  it.first -= n;
  it.second -= n;
}

template <typename Iterator> auto increment(Iterator &it) { ++it; }

template <typename Iterator0, typename Iterator1>
auto increment(std::pair<Iterator0, Iterator1> &it) {
  ++it.first;
  ++it.second;
}

template <typename Iterator> auto decrement(Iterator &it) { --it; }

template <typename Iterator0, typename Iterator1>
auto decrement(std::pair<Iterator0, Iterator1> &it) {
  --it.first;
  --it.second;
}

template <typename Iterator>
auto difference(const Iterator &it0, const Iterator &it1) {
  return it0 - it1;
}

template <typename Iterator0, typename Iterator1>
auto difference(const std::pair<Iterator0, Iterator1> &it0,
                const std::pair<Iterator0, Iterator1> &it1) {
  return it0.first - it1.first;
}
} // namespace tf::implementation::iter
