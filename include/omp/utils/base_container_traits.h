#pragma once

#include <iterator>
#include <type_traits>
#include <utility>

namespace omp {

namespace details {

template <typename T> struct add_const_to_type {
  using type = const T;
};

template <typename T>
using add_const_to_type_t = typename add_const_to_type<T>::type;

template <typename T> struct add_const_to_type<T &> {
  using type = add_const_to_type_t<T> &;
};

template <typename T> struct add_const_to_type<T &&> {
  using type = add_const_to_type_t<T> &&;
};

using std::begin;

template <typename Container>
using iterator = decltype(begin(std::declval<Container &>()));

template <typename Container>
using const_iterator =
    decltype(begin(std::declval<add_const_to_type_t<Container> &>()));

} // namespace details

template <typename Container> struct base_container_traits {
  using container = Container;

  using iterator = details::iterator<Container>;
  using const_iterator = details::const_iterator<Container>;
};

} // namespace omp
