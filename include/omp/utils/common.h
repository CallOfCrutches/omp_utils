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
    collector( const T&, F&& ) -> collector<T, F>;

    template<typename Tuple>
    constexpr bool support_get_one_( decltype( std::get<0>( std::declval<Tuple>() ), int() ) ) { return true; }
    constexpr bool support_get_one_( ... ) { return false; }

    template<typename Tuple, typename... Tuples>
    constexpr bool support_get_()
    {
      return support_get_one_<Tuple>( 0 ) && ( support_get_one_<Tuples>( 0 ) && ... );
    }

    template<typename Tuple, typename... Tuples>
    constexpr bool same_size_()
    {
      return std::tuple_size_v<std::remove_reference_t<Tuple>> &&
        ( ( std::tuple_size_v<std::remove_reference_t<Tuple>> == std::tuple_size_v<std::remove_reference_t<Tuples>> )
          && ... );
    }

    template<std::size_t Idx, typename Callable, typename... Tuples>
    decltype( auto ) map_one( Callable&& call, Tuples&&... tups )
    {
      return std::forward<Callable>( call )( std::get<Idx>( std::forward<Tuples>( tups ) )... );
    }

    template<std::size_t Idx, typename Callable, typename Value, typename... Tuples>
    decltype( auto ) reduce_one( Callable&& call, Value&& initial, Tuples&&... tups )
    {
      return std::forward<Callable>( call )( initial, std::get<Idx>( std::forward<Tuples>( tups )... ) );
    }

    template<std::size_t... Idxs, typename Callable, typename Value, typename... Tuples>
    auto tuple_reduce_( std::index_sequence<Idxs...>, Callable&& call, Value&& initial, Tuples&&... tups )
    {
    }

    template<std::size_t... Idxs, typename Callable, typename... Tuples>
    auto tuple_map_( std::index_sequence<Idxs...>, Callable&& call, Tuples&&... tups )
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
    std::enable_if_t<details::support_get_<Tuples...>() && details::same_size_<Tuples...>(), int> = 0>
  auto tuple_map( Callable&& call, Tuples&&... tups )
  {
    using FirstTuple = std::remove_reference_t<std::tuple_element_t<0, std::tuple<Tuples...>>>;
    return details::tuple_map_( std::make_index_sequence<std::tuple_size_v<FirstTuple>>(),
                                std::forward<Callable>( call ), std::forward<Tuples>( tups )... );
  }

  template<typename Callable, typename Value, typename... Tuples>
  auto tuple_reduce( Callable&& call, Value&& initial, Tuples&&... tups )
  {
    return nullptr;
  }
}
