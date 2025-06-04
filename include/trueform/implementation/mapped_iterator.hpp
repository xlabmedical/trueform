/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include "./dereference_policy.hpp"
#include <iterator>
namespace tf::implementation::iter {

template <typename Derived, typename Iterator, typename DereferncePolicy,
          bool defer = false>
struct forward_mapped_crtp : protected dereference_policy<DereferncePolicy> {
private:
  template <typename It, typename DP, bool> struct reference_helper {
    using type = decltype(std::declval<DP>()(*std::declval<It>()));
  };

  template <typename It, typename DP> struct reference_helper<It, DP, true> {
    using type =
        decltype(std::declval<DereferncePolicy>()(std::declval<Iterator>()));
  };

protected:
  using dref_base_t = dereference_policy<DereferncePolicy>;
  using reference_t =
      typename reference_helper<Iterator, DereferncePolicy, defer>::type;

  Iterator iter;

  auto as_derived() const -> const Derived & {
    return static_cast<const Derived &>(*this);
  }

  auto as_derived() -> Derived & { return static_cast<Derived &>(*this); }

public:
  using iterator_category =
      typename std::iterator_traits<Iterator>::iterator_category;
  using reference = reference_t;
  using value_type = std::decay_t<reference_t>;
  using pointer =
      std::conditional_t<std::is_reference_v<reference>,
                         std::remove_reference_t<reference> *, void>;
  using difference_type =
      typename std::iterator_traits<Iterator>::difference_type;

  forward_mapped_crtp(Iterator iter, DereferncePolicy dereference_policy)
      : dref_base_t{std::move(dereference_policy)}, iter(std::move(iter)) {}

  forward_mapped_crtp() = default;

  auto base_iter() const { return iter; }

  auto operator*() const -> decltype(auto) {
    if constexpr (defer)
      return static_cast<const dref_base_t &>(*this)(iter);
    else
      return static_cast<const dref_base_t &>(*this)(*iter);
  }

  template <typename T = reference_t,
            typename = std::enable_if_t<std::is_reference<T>::value, void>>
  auto operator->() const {
    if constexpr (defer)
      return std::addressof(static_cast<const dref_base_t &>(*this)(*iter));
    else
      return std::addressof(static_cast<const dref_base_t &>(*this)(*iter));
  }

  auto operator++() -> Derived & {
    ++iter;
    return static_cast<Derived &>(*this);
  }

  auto operator++(int) -> Derived {
    auto out = static_cast<Derived &>(*this);
    ++static_cast<Derived &>(*this);
    return out;
  }

  friend auto operator==(const Derived &obj, const Derived &other) {
    return obj.iter == other.iter;
  }
  friend auto operator!=(const Derived &obj, const Derived &other) {
    return obj.iter != other.iter;
  }
};

template <typename Derived, typename Iterator, typename DereferncePolicy,
          bool defer = false>
struct bidirectional_mapped_crtp
    : forward_mapped_crtp<Derived, Iterator, DereferncePolicy, defer> {
private:
  using base_t =
      forward_mapped_crtp<Derived, Iterator, DereferncePolicy, defer>;

public:
  using base_t::base_t;

  auto operator--() -> Derived & {
    --base_t::iter;
    return static_cast<Derived &>(*this);
  }

  auto operator--(int) -> Derived {
    auto out = static_cast<Derived &>(*this);
    --static_cast<Derived &>(*this);
    return out;
  }
};

template <typename Derived, typename Iterator, typename DereferncePolicy,
          bool defer = false>
struct random_access_mapped_crtp
    : bidirectional_mapped_crtp<Derived, Iterator, DereferncePolicy, defer> {
private:
  using base_t =
      bidirectional_mapped_crtp<Derived, Iterator, DereferncePolicy, defer>;

public:
  using base_t::base_t;

  auto operator[](typename base_t::difference_type n) const -> decltype(auto) {
    return *(*this + n);
  }

  auto operator+=(typename base_t::difference_type n) -> Derived & {
    base_t::iter += n;
    return static_cast<Derived &>(*this);
  }

  auto operator+(typename base_t::difference_type n) const -> Derived {
    auto out = static_cast<const Derived &>(*this);
    out += n;
    return out;
  }

  friend auto operator+(typename base_t::difference_type n, const Derived &it) {
    return it + n;
  }

  auto operator-=(typename base_t::difference_type n) -> Derived {
    base_t::iter -= n;
    return static_cast<Derived &>(*this);
  }

  auto operator-(typename base_t::difference_type n) const -> Derived {
    auto out = static_cast<const Derived &>(*this);
    out -= n;
    return out;
  }

  friend auto operator-(const Derived &obj, const Derived &other) {
    return obj.iter - other.iter;
  }

  friend auto operator<(const Derived &obj, const Derived &other) {
    return obj.iter < other.iter;
  }
  friend auto operator<=(const Derived &obj, const Derived &other) {
    return obj.iter <= other.iter;
  }
  friend auto operator>(const Derived &obj, const Derived &other) {
    return obj.iter > other.iter;
  }
  friend auto operator>=(const Derived &obj, const Derived &other) {
    return obj.iter >= other.iter;
  }
};

template <typename Tag, typename Derived, typename Iterator,
          typename DereferncePolicy, bool defer>
struct mapped_crtp_picker {
  using type = forward_mapped_crtp<Derived, Iterator, DereferncePolicy, defer>;
};

template <typename Derived, typename Iterator, typename DereferncePolicy,
          bool defer>
struct mapped_crtp_picker<std::forward_iterator_tag, Derived, Iterator,
                          DereferncePolicy, defer> {
  using type = forward_mapped_crtp<Derived, Iterator, DereferncePolicy, defer>;
};

template <typename Derived, typename Iterator, typename DereferncePolicy,
          bool defer>
struct mapped_crtp_picker<std::bidirectional_iterator_tag, Derived, Iterator,
                          DereferncePolicy, defer> {
  using type =
      bidirectional_mapped_crtp<Derived, Iterator, DereferncePolicy, defer>;
};

template <typename Derived, typename Iterator, typename DereferncePolicy,
          bool defer>
struct mapped_crtp_picker<std::random_access_iterator_tag, Derived, Iterator,
                          DereferncePolicy, defer> {
  using type =
      random_access_mapped_crtp<Derived, Iterator, DereferncePolicy, defer>;
};

template <typename Iterator, typename DereferncePolicy>
struct mapped : mapped_crtp_picker<
                    typename std::iterator_traits<Iterator>::iterator_category,
                    mapped<Iterator, DereferncePolicy>, Iterator,
                    DereferncePolicy, false>::type {

private:
  using base_t = typename mapped_crtp_picker<
      typename std::iterator_traits<Iterator>::iterator_category,
      mapped<Iterator, DereferncePolicy>, Iterator, DereferncePolicy,
      false>::type;

public:
  using base_t::base_t;
};

template <typename Iterator, typename DereferncePolicy>
auto make_mapped(Iterator iter, DereferncePolicy &&dereference_policy) {
  return mapped<Iterator, std::decay_t<DereferncePolicy>>{
      std::move(iter), static_cast<DereferncePolicy &&>(dereference_policy)};
}

template <typename Iterator, typename DereferncePolicy>
struct iter_mapped
    : mapped_crtp_picker<
          typename std::iterator_traits<Iterator>::iterator_category,
          iter_mapped<Iterator, DereferncePolicy>, Iterator, DereferncePolicy,
          true>::type {

private:
  using base_t = typename mapped_crtp_picker<
      typename std::iterator_traits<Iterator>::iterator_category,
      iter_mapped<Iterator, DereferncePolicy>, Iterator, DereferncePolicy,
      true>::type;

public:
  using base_t::base_t;
};

template <typename Iterator, typename DereferncePolicy>
auto make_iter_mapped(Iterator iter, DereferncePolicy &&dereference_policy) {
  return iter_mapped<Iterator, std::decay_t<DereferncePolicy>>{
      std::move(iter), static_cast<DereferncePolicy &&>(dereference_policy)};
}
} // namespace tf::implementation::iter
