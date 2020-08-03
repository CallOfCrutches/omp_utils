#pragma once

#include "common.h"
#include <type_traits>


namespace omp
{
  namespace details
  {
    namespace functors
    {
      struct indirection
      {
        template<typename Iterator>
        auto& operator()( Iterator& iter ) { return *iter; }
      };
      
      struct increment
      {
        template<typename Iterator>
        auto operator()( Iterator iter ) { return ++iter; }
      };
      
      struct are_same
      {
        template<typename First, typename Second>
        bool operator()( bool same, const First& first, const Second& second )
        {
          return same || ( first == second );
        }
      };
      
      struct begin
      {
        template<typename Container>
        auto operator()( Container& container )
        {
          using std::begin;
          return begin( container );
        }
      };
      
      struct end
      {
        template<typename Container>
        auto operator()( Container& container )
        {
          using std::end;
          return end( container );
        }
      };

      struct cbegin
      {
        template<typename Container>
        auto operator()( const Container& container )
        {
          using std::begin;
          return begin( container );
        }
      };
      
      struct cend
      {
        template<typename Container>
        auto operator()( const Container& container )
        {
          using std::end;
          return end( container );
        }
      };
    }

    template<typename T>
    struct make_const_if
    {
        using type = std::add_const_t<std::remove_reference_t<T>>;
    };

    template<typename T>
    struct make_const_if<T&>
    {
        using type = T&;
    };

    template<typename T>
    using make_const_if_t = typename make_const_if<T>::type;

    template<typename... Iterators>
    class zip_iterator
    {
    public:
      using iterator_category = std::forward_iterator_tag;
      using difference_type = std::ptrdiff_t;

      using value_type = decltype( omp::tuple_map( functors::indirection(),
                                   std::declval<std::tuple<Iterators...>&>() ) );

      using reference = const value_type;

      template<typename Tuple>
      struct proxy
      {
        proxy( Tuple&& values ) noexcept( std::is_nothrow_move_constructible_v<Tuple>() )
          : values( std::move( values ) )
        {}

        const auto operator->() const noexcept
        {
          return &values;
        }

        Tuple values;
      };

      using pointer = proxy<value_type>;

      zip_iterator( Iterators... iterators )
        : iterators( std::forward<Iterators>( iterators )... )
      {}

      template<typename... InIterators>
      zip_iterator( const std::tuple<InIterators...>& iterators )
          : iterators( iterators )
      {}

      template<typename... InIterators>
      zip_iterator( std::tuple<InIterators...>&& iterators )
        : iterators( std::move( iterators ) )
      {}

      pointer operator->() const
      {
        return omp::tuple_map(functors::indirection(), iterators );
      }

      reference operator*() const
      {
        return omp::tuple_map(functors::indirection(), iterators );
      }

      zip_iterator& operator++()
      {
        iterators = omp::tuple_map(functors::increment(), std::move( iterators ) );

        return *this;
      }

      zip_iterator operator++( int )
      {
        auto temp = *this;
        ++( *this );

        return temp;
      }

      bool operator==( const zip_iterator& rhs ) const
      {
        return omp::tuple_reduce( functors::are_same(), false, iterators, rhs.iterators );
      }

      bool operator!=( const zip_iterator& rhs ) const
      {
        return !( *this == rhs );
      }

    private:
      std::tuple<Iterators...> iterators;
    };

    template<typename... Iterators>
    zip_iterator( Iterators... iterators ) -> zip_iterator<Iterators...>;

    template<typename... Iterators>
    zip_iterator( const std::tuple<Iterators...>& iterators ) -> zip_iterator<std::remove_reference_t<Iterators>...>;

    template<typename... Iterators>
    zip_iterator( std::tuple<Iterators...>&& iterators ) -> zip_iterator<std::remove_reference_t<Iterators>...>;
  }

  using details::zip_iterator;

  template<typename... Containers>
  struct zip
  {
    using iterator = zip_iterator<decltype( omp::tuple_map( details::functors::begin(),
                                            std::declval<std::tuple<Containers...>&>() ) )>;

    using const_iterator = zip_iterator<decltype( omp::tuple_map( details::functors::cbegin(),
                                                  std::declval<std::tuple<Containers...>&>()))>;

    zip( Containers... containers )
      : containers( std::forward<Containers>( containers )... )
    {}

    iterator begin()
    {
      return omp::tuple_map( details::functors::begin(), containers );
    }

    iterator end()
    {
      return omp::tuple_map( details::functors::end(), containers );
    }

    const_iterator begin() const
    {
      return omp::tuple_map( details::functors::cbegin(), containers );
    }

    const_iterator end() const
    {
      return omp::tuple_map( details::functors::cend(), containers );
    }

    const_iterator cbegin() const
    {
      return begin();
    }

    const_iterator cend() const
    {
      return end();
    }

  private:
    std::tuple<Containers...> containers;
  };

  template<typename... Containers>
  zip( Containers&&... containers ) -> zip<details::make_const_if_t<Containers>...>;
}
