#pragma once
#include <type_traits>

namespace utils
{
  namespace detail
  {
    template<typename Dummy>
    constexpr std::false_type has_reverse( ... );

    template<typename Container>
    constexpr std::true_type has_reverse( 
      decltype( std::declval<Container>().rbegin(), 
                std::declval<Container>().rend() )* );

    template<typename Container>
    struct reversed_container_adapter
    {

      static_assert( decltype( has_reverse<Container>( nullptr ) )(),
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

      auto rbegin() noexcept
      {
        return container.begin();
      }

      auto rbegin() const noexcept
      {
        return container.begin();
      }

      auto rend() noexcept
      {
        return container.end();
      }

      auto rend() const noexcept
      {
        return container.end();
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
