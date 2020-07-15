#pragma once

#include <utility>
#include <tuple>


namespace omp
{

  namespace details
  {
    // Thought: maybe change this type reduce compatible interface?
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
      collector( T value, F func ) noexcept
        : value( std::move( value ) )
        , func( std::move( func ) )
      {}

      template<typename V>
      auto operator|( const collector_value<V>& cvalue )
      {
        using common_t = std::common_type_t<T, V>;
        // TODO: make something with types
        using collector_type = decltype( func( static_cast<common_t>( value ),
                                               static_cast<common_t>( cvalue.value ) ) );

        return collector<collector_type, F>( func( static_cast<common_t>( value ),
                                                   static_cast<common_t>( cvalue.value ) ),
                                             std::move( func ) );
      }

      T value;
      F func;
    };

    template<typename T, typename F>
    collector( const T&, F&& )->collector<T, F>;

    template<typename Callable, typename... Types, std::size_t... Idxs>
    auto tuple_map_( Callable&& call, const std::tuple<Types...>& tup, std::index_sequence<Idxs...> )
    {
      return std::make_tuple( call( std::get<Idxs>( tup ) )... );
    }

    template<typename Callable, typename... Types, std::size_t... Idxs>
    auto tie_map_( Callable&& call, std::tuple<Types...>& tup, std::index_sequence<Idxs...> )
    {
      return std::tie( call( std::get<Idxs>( tup ) )... );
    }
  }

  template<typename First, typename... Others>
  auto min( const First& first, const Others&... others )
  {
    using details::collector;
    using details::collector_value;

    auto min_generic = []( const auto& f, const auto& s ) { return f < s ? f : s; };

    return ( collector( first, std::move( min_generic ) ) | ... | collector_value<Others>( others ) ).value;
  }
  
  template<typename First, typename... Others>
  auto max( const First& first, const Others&... others )
  {
    using details::collector;
    using details::collector_value;

    auto max_generic = []( const auto& f, const auto& s ) { return f > s ? f : s; };

    return ( collector( first, std::move( max_generic ) ) | ... | collector_value<Others>( others ) ).value;
  }

  template<typename First, typename... Others>
  auto sum( const First& first, const Others&... others )
  {
    using details::collector;
    using details::collector_value;

    auto plus_generic = []( const auto& f, const auto& s ) { return f + s; };

    return ( collector( first, std::move( plus_generic ) ) | ... | collector_value<Others>( others ) ).value;
  }

  // Make working for many args like tuples. 
  // Hint: std::get<>.
  template<typename Callable, typename... Types>
  auto tuple_map( Callable&& call, const std::tuple<Types...>& tup )
  {
    return details::tuple_map_( std::forward<Callable>( call ), tup, std::index_sequence_for<Types...>() );
  }

  template<typename Callable, typename... Types>
  auto tie_map( Callable&& call, std::tuple<Types...>& tup )
  {
    return details::tie_map_( std::forward<Callable>( call ), tup, std::index_sequence_for<Types...>() );
  }

  template<typename... Types, typename Callable, typename Value>
  auto tuple_reduce( Callable&& call, const std::tuple<Types...>& tup, Value&& initial )
  {
    return nullptr;
  }
}
