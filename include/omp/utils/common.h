#pragma once

#include <utility>
#include <type_traits>

namespace omp
{

  namespace details
  {

    template<typename T>
    struct collector_value
    {
      explicit collector_value( const T& value ) noexcept
        : value( value )
      {}

      const T& value;
    };

    template<typename T, typename F>
    struct collector
    {
      collector( const T& value, F func ) noexcept
        : value( value )
        , func( std::move( func ) )
      {}

      template<typename V>
      auto operator|( const collector_value<V>& cvalue )
      {
        using common_t = std::common_type_t<T, V>;
        // TODO: make something with types
        using collector_type = decltype( func( static_cast<common_t>( value.value ),
                                               static_cast<common_t>( cvalue.value ) ) );

        return collector<collector_type, F>( func( static_cast<common_t>( value.value ),
                                                   static_cast<common_t>( cvalue.value ) ),
                                             std::move( func ) );
      }

      operator T() { return value.value; }

      collector_value<T> value;
      F func;
    };

    template<typename T, typename F>
    collector( const T&, F&& )->collector<T, F>;

  }

  template<typename First, typename... Others>
  auto min( const First& first, const Others&... others )
  {
    using details::collector;
    using details::collector_value;

    auto min_generic = []( const auto& f, const auto& s ) { return f < s ? f : s; };

    return ( collector( first, std::move( min_generic ) ) | ... | collector_value<Others>( others ) );
  }
  
  template<typename First, typename... Others>
  auto max( const First& first, const Others&... others )
  {
    using details::collector;
    using details::collector_value;

    auto max_generic = []( const auto& f, const auto& s ) { return f > s ? f : s; };

    return ( collector( first, std::move( max_generic ) ) | ... | collector_value<Others>( others ) );
  }

  template<typename First, typename... Others>
  auto sum( const First& first, const Others&... others )
  {
    using details::collector;
    using details::collector_value;

    auto plus_generic = []( const auto& f, const auto& s ) { return f + s; };

    return ( collector( first, std::move( plus_generic ) ) | ... | collector_value<Others>( others ) );
  }
}
