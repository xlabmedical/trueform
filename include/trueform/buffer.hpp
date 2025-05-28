/*
 * Copyright (c) 2025 Žiga Sajovic, XLAB
 * Distributed under the Boost Software License, Version 1.0.
 * https://github.com/xlabmedical/trueform
 */
#pragma once
#include <cstring>
#include <memory>
#include <type_traits>
#include <utility>

namespace tf {
template <typename T> class buffer {
  static_assert(std::is_trivially_default_constructible<T>::value &&
                    std::is_trivially_destructible<T>::value,
                "Just use std::vector");

public:
  buffer() = default;

  buffer(const buffer &other) {
    auto size = other.size();
    allocate(size);
    std::memcpy(_data.get(), other._data.get(), size * sizeof(T));
  }

  buffer(buffer &&other) noexcept {
    _data = std::move(other._data);
    _end = other._end;
    _capacity = other._capacity;
    other._end = other._data.get();
    other._capacity = other._data.get();
  }

  auto operator=(const buffer &other) -> buffer & {
    auto size = other.size();
    allocate(size);
    std::memcpy(_data.get(), other._data.get(), size * sizeof(T));
    return *this;
  }

  auto operator=(buffer &&other) noexcept -> buffer & {
    _data = std::move(other._data);
    _end = other._end;
    _capacity = other._capacity;
    other._end = other._data.get();
    other._capacity = other._data.get();
    return *this;
  }

  auto allocate(std::size_t n) {
    if (n <= capacity()) {
      _end = _data.get() + n;
    } else {
      auto new_capacity = compute_new_capacity(n - size());
      std::unique_ptr<T[]> tmp{new T[new_capacity]};
      std::swap(tmp, _data);
      _end = _data.get() + n;
      _capacity = _data.get() + new_capacity;
    }
  }

  auto reallocate(std::size_t n) {
    if (n <= capacity()) {
      _end = _data.get() + n;
    } else
      append_at_end(n - size());
  }

  auto reallocate_and_initialize(std::size_t n, T t) {
    if (n <= capacity()) {
      _end = _data.get() + n;
    } else {
      auto _size = size();
      append_at_end(n - _size);
      std::fill(_data.get() + _size, _end, std::move(t));
    }
  }

  auto allocate_and_initialize(std::size_t n, T t) {
    allocate(n);
    std::fill(_data.get(), _end, std::move(t));
  }

  auto erase_till_end(const T *_new_end) { _end = _new_end; }

  auto erase(const T *_from, const T *_to) {
    auto it = _to;
    while (it != _end) {
      *_from++ = *it++;
    }
    _end = _from;
    return _end;
  }

  auto push_back(const T &t) -> T & {
    append_at_end(1);
    *(_end - 1) = t;
    return *(_end - 1);
  }

  auto push_back(T &&t) -> T & {
    append_at_end(1);
    *(_end - 1) = std::move(t);
    return *(_end - 1);
  }

  auto pop_back() { --_end; }

  template <typename... Us> auto emplace_back(Us &&...us) -> T & {
    append_at_end(1);
    new (_end - 1) T{static_cast<Us &&>(us)...};
    return *(_end - 1);
  }

  auto clear() { _end = _data.get(); }

  auto reserve(std::size_t n) {
    auto _size = size();
    if (n > _size) {
      append_at_end(n - _size, n);
      _end = _data.get() + _size;
    }
  }

  auto reset() {
    _data.reset(nullptr);
    _end = _data.get();
    _capacity = _end;
  }

  auto release() {
    auto out = _data.release();
    _end = _data.get();
    _capacity = _end;
    return out;
  }

  auto operator[](std::size_t i) const -> const T & { return *(begin() + i); }

  auto operator[](std::size_t i) -> T & { return *(begin() + i); }

  auto capacity() const -> std::size_t { return _capacity - _data.get(); }

  auto size() const -> std::size_t { return _end - _data.get(); }

  // Range-compatible iterators
  auto begin() -> T * { return _data.get(); }
  auto end() -> T * { return _end; }

  auto begin() const -> const T * { return _data.get(); }
  auto end() const -> const T * { return _end; }

  auto cbegin() const -> const T * { return _data.get(); }
  auto cend() const -> const T * { return _end; }

  // Front/back access
  auto front() -> T & { return *_data.get(); }
  auto front() const -> const T & { return *_data.get(); }

  auto back() -> T & { return *(_end - 1); }
  auto back() const -> const T & { return *(_end - 1); }

private:
  auto append_at_end(std::size_t n, std::size_t new_capacity) {
    auto _old_size = size();
    std::unique_ptr<T[]> tmp{new T[new_capacity]};
    std::memcpy(tmp.get(), _data.get(), _old_size * sizeof(T));
    std::swap(tmp, _data);
    _end = _data.get() + _old_size + n;
    _capacity = _data.get() + new_capacity;
  }

  auto append_at_end(std::size_t n) {
    auto _old_size = size();
    if (capacity() - _old_size >= n) {
      _end = _end + n;
    } else {
      auto new_capacity = compute_new_capacity(n);
      append_at_end(n, new_capacity);
    }
  }

  auto compute_new_capacity(std::size_t added_elements) {
    return size() + std::max(size(), added_elements);
  }

  std::unique_ptr<T[]> _data = nullptr;
  T *_end = nullptr;
  T *_capacity = nullptr;
};
} // namespace tf
