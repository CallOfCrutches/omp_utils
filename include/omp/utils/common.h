#pragma once

#include <utility>
#include <tuple>




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
    collector( const T&, F&& ) -> collector<T, F>;

    template<typename Tuple>
    constexpr bool support_get_one_( decltype( std::get<0>( std::declval<Tuple>() ), int() ) ) { return true; }
    template<typename>
    constexpr bool support_get_one_( ... ) { return false; }

    template<typename... Tuples>
    constexpr bool support_get_()
    {
      return ( ... && support_get_one_<Tuples>( 0 ) );
    }

    template<typename First, typename... Rest> First first_type_();

     template<typename> struct tuple_size : std::integral_constant<std::size_t, 0> {};

    template<typename... Tuples>
    constexpr bool same_size_()
    {
      using details::tuple_size;

      using FirstTuple = std::remove_reference_t<decltype(first_type_<Tuples..., void>())>;
      return ( ... && ( tuple_size<FirstTuple>::value == tuple_size<std::remove_reference_t<Tuples>>::value ) );
    }

    template<std::size_t Idx, typename Callable, typename... Tuples>
    constexpr decltype( auto ) map_one( Callable&& call, Tuples&&... tups )
    {
      return call( std::get<Idx>( std::forward<Tuples>( tups ) )... );
    }

    template<std::size_t... Idxs, typename Callable, typename... Tuples>
    constexpr auto tuple_map_( std::index_sequence<Idxs...>, Callable&& call, Tuples&&... tups )
    {
      if constexpr( ( std::is_reference_v<decltype( map_one<Idxs>(
        std::forward<Callable>( call ), std::forward<Tuples>( tups )... ) )> && ... ) )
      {
        return std::tie( map_one<Idxs>( std::forward<Callable>( call ), std::forward<Tuples>( tups )... )... );
      }
      else
      {
        return std::make_tuple( map_one<Idxs>( std::forward<Callable>( call ), std::forward<Tuples>( tups )... )... );
      }
    }

    template<std::size_t Idx, typename Callable, typename Value, typename... Tuples>
    constexpr void reduce_one( Callable&& call, Value& value, Tuples&&... tups )
    {
      value = call( std::move( value ), std::get<Idx>( std::forward<Tuples>( tups ) )... );
    }

    template<std::size_t... Idxs, typename Callable, typename Value, typename... Tuples>
    constexpr auto tuple_reduce_( std::index_sequence<Idxs...>, Callable&& call, Value initial, Tuples&&... tups )
    {
      ( reduce_one<Idxs>( std::forward<Callable>( call ), initial, std::forward<Tuples>( tups )... ), ... );

      return initial;
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

  template<typename Callable, typename... Tuples,
    std::enable_if_t<sizeof...(Tuples) && details::support_get_<Tuples...>() && details::same_size_<Tuples...>(),
                     int> = 0>
  constexpr auto tuple_map( Callable&& call, Tuples&&... tups )
  {
    using FirstTuple = std::remove_reference_t<decltype( details::first_type_<Tuples...>() )>;
    return details::tuple_map_( std::make_index_sequence<std::tuple_size_v<FirstTuple>>(),
                                std::forward<Callable>( call ), std::forward<Tuples>( tups )... );
  }
  
  template<typename Callable, typename Value, typename... Tuples,
    std::enable_if_t<sizeof...(Tuples) && details::support_get_<Tuples...>() && details::same_size_<Tuples...>(),
                     int> = 0>
  constexpr auto tuple_reduce( Callable&& call, Value&& initial, Tuples&&... tups )
  {
    using FirstTuple = std::remove_reference_t<decltype( details::first_type_<Tuples...>() )>;
    return details::tuple_reduce_( std::make_index_sequence<std::tuple_size_v<FirstTuple>>(),
                                   std::forward<Callable>( call ), std::forward<Value>( initial ),
                                   std::forward<Tuples>( tups )... );
  }
}
