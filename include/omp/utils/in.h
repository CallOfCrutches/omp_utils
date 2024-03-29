#pragma once

#include <algorithm>

namespace omp {
namespace details {

template <typename ValueType, typename ContainerType>
bool in_(const ValueType &value, const ContainerType &container,
         decltype(std::declval<ContainerType>().find(
             std::declval<typename ContainerType::key_type>())) *) {
  using std::end;

  return container.find(value) != end(container);
}

template <typename ValueType, typename ContainerType>
bool in_(const ValueType &value, const ContainerType &container, ...) {
  using std::begin;
  using std::end;

  return std::find(begin(container), end(container), value) != end(container);
}

} // namespace details

template <typename ValueType, typename ArgType1, typename ArgType2,
          typename... ArgTypes>
bool in(const ValueType &value, const ArgType1 &arg1, const ArgType2 &arg2,
        const ArgTypes &...args) {
  return value == arg1 || value == arg2 || ((value == args) || ...);
}

template <typename ValueType, typename ContainerType>
bool in(const ValueType &value, const ContainerType &container) {
  return details::in_(value, container, nullptr);
}

} // namespace omp
