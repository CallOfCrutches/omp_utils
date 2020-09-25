#pragma once

#include <utility>
#include <tuple>


namespace omp
{

  namespace details
  {
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
        return std::tuple<decltype( map_one<Idxs>(
            std::forward<Callable>( call ), std::forward<Tuples>( tups )... ) )...>(
                map_one<Idxs>( std::forward<Callable>( call ), std::forward<Tuples>( tups )... )... );
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
