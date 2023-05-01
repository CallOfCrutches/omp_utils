#pragma once

#include "base_container_traits.h"
#include <iterator>

namespace omp {
namespace details {

template <typename Iterator> struct enumerate_iterator {
  using wrapped_iterator = Iterator;

  using iterator_category = std::forward_iterator_tag;

  using value_type =
      std::pair<const std::ptrdiff_t,
                typename std::iterator_traits<wrapped_iterator>::reference>;

  using difference_type = std::ptrdiff_t;

  struct pointer {
    explicit pointer(value_type &&value) noexcept : value_(std::move(value)) {}

    value_type *operator->() noexcept { return &value_; }

    value_type value_;
  };

  using reference = const value_type;

  explicit enumerate_iterator(
      wrapped_iterator &&iter,
      std::ptrdiff_t
          value) noexcept(std::
                              is_nothrow_move_constructible_v<wrapped_iterator>)
      : iterator_(std::move(iter)), value_(value) {}

  reference operator*() noexcept { return value_type(value_, *iterator_); }

  pointer operator->() noexcept {
    return pointer(value_type(value_, *iterator_));
  }

  enumerate_iterator &operator++() {
    ++iterator_;
    ++value_;

    return *this;
  }

  enumerate_iterator operator++(int) {
    auto tmp = *this;

    ++(*this);

    return tmp;
  }

  bool operator==(const enumerate_iterator &rhs) const noexcept {
    return iterator_ == rhs.iterator_;
  }

  bool operator!=(const enumerate_iterator &rhs) const noexcept {
    return !(*this == rhs);
  }

  wrapped_iterator iterator_;
  std::ptrdiff_t value_;
};

template <typename Value> struct enumerate_pointer_adapter {
  using iterator = Value *;
  using const_iterator = const Value *;

  explicit enumerate_pointer_adapter(Value *begin, Value *end) noexcept
      : begin_(begin), end_(end) {}

  iterator begin() noexcept { return begin_; }

  iterator end() noexcept { return end_; }

  const_iterator begin() const noexcept { return begin_; }

  const_iterator end() const noexcept { return end_; }

  const_iterator cbegin() const noexcept { return begin(); }

  const_iterator cend() const noexcept { return end(); }

private:
  Value *begin_;
  Value *end_;
};

template <typename Container> struct enumerate {
  using traits = base_container_traits<Container>;

  using container_iterator = typename traits::iterator;
  using const_container_iterator = typename traits::const_iterator;

  using iterator = enumerate_iterator<container_iterator>;
  using const_iterator = enumerate_iterator<const_container_iterator>;

  using value_type = typename iterator::value_type;
  using difference_type = typename iterator::difference_type;

  explicit enumerate(Container container, std::ptrdiff_t start = {})
      : container_(std::forward<Container>(container)), start_(start) {}

  iterator begin() noexcept { return iterator(std::begin(container_), start_); }

  iterator end() noexcept { return iterator(std::end(container_), start_); }

  const_iterator begin() const noexcept {
    return const_iterator(std::cbegin(container_), start_);
  }

  const_iterator end() const noexcept {
    return const_iterator(std::cend(container_), start_);
  }

  const_iterator cbegin() const noexcept { return begin(); }

  const_iterator cend() const noexcept { return end(); }

private:
  std::ptrdiff_t start_{};
  Container container_;
};

template <typename ContainerType>
enumerate(ContainerType &&) -> enumerate<ContainerType>;

template <typename ContainerType>
enumerate(ContainerType &&, std::ptrdiff_t) -> enumerate<ContainerType>;

} // namespace details

template <typename ContainerType>
auto enumerate(ContainerType &&container, std::ptrdiff_t start = 0) {
  return details::enumerate(std::forward<ContainerType>(container), start);
}

template <typename ValueType, size_t size>
auto enumerate(ValueType (&arr)[size], std::ptrdiff_t start = 0) {
  return details::enumerate(details::enumerate_pointer_adapter(arr, arr + size),
                            start);
}

template <typename ValueType, size_t size>
auto enumerate(ValueType (&&arr)[size], std::ptrdiff_t start = 0) = delete;

template <typename ValueType>
auto enumerate(ValueType *begin, ValueType *end, std::ptrdiff_t start = 0) {
  return details::enumerate(details::enumerate_pointer_adapter(begin, end),
                            start);
}

} // namespace omp
