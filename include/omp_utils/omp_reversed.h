#pragma once
#include <type_traits>

namespace utils
{
  namespace detail
  {

#ifndef __clang__
    template<typename... Ts>
    using _void_t = std::void_t<Ts...>;
#else // CWG 1558
    template<typename...>
    struct _void_struct { using type = void; };

    template<typename... Ts>
    using _void_t = typename _void_struct<Ts...>::type;
#endif

    template<typename T, typename = void>
    struct can_be_reversed: public std::false_type {};

    template<typename T>
    struct can_be_reversed<T, _void_t<typename T::reverse_iterator>>
      : public std::true_type {};

    template<typename Container>
    struct reversed_container_adapter
    {

      static_assert( can_be_reversed<std::decay_t<Container>>::value,
                     "This type doesn't support reverse operation" );

      template<typename Container = Container>
      reversed_container_adapter( Container&& container )
        : container( std::forward<Container>( container ) )
      { }

      auto begin() noexcept
      {
        return container.rbegin();
      }

      auto begin() const noexcept
      {
        return container.rbegin();
      }

      auto end() noexcept
      {
        return container.rend();
      }

      auto end() const noexcept
      {
        return container.rend();
      }

    private:
      Container container;
    };
  }

  template<typename Container>
  auto reversed( Container&& container )
  {
    return detail::reversed_container_adapter<Container>(
      std::forward<Container>( container ) );
  }
}
