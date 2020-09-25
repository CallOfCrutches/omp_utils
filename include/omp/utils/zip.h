#pragma once

#include "common.h"
#include <type_traits>


namespace omp
{
  namespace details
  {
    namespace functor
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

    template<typename... Iterators>
    class zip_iterator
    {
    public:
      using iterator_category = std::forward_iterator_tag;
      using difference_type = std::ptrdiff_t;

      using value_type = decltype( omp::tuple_map( functor::indirection(),
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
        : iterators_( std::forward<Iterators>( iterators )... )
      {}

      template<typename... InIterators>
      zip_iterator( const std::tuple<InIterators...>& iterators )
          : iterators_( iterators )
      {}

      template<typename... InIterators>
      zip_iterator( std::tuple<InIterators...>&& iterators )
        : iterators_( std::move( iterators ) )
      {}

      pointer operator->() const
      {
        return omp::tuple_map( functor::indirection(), iterators_ );
      }

      reference operator*() const
      {
        return omp::tuple_map( functor::indirection(), iterators_ );
      }

      zip_iterator& operator++()
      {
        iterators_ = omp::tuple_map( functor::increment(), std::move( iterators_ ) );

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
        return omp::tuple_reduce( functor::are_same(), false, iterators_, rhs.iterators_ );
      }

      bool operator!=( const zip_iterator& rhs ) const
      {
        return !( *this == rhs );
      }

      auto& iterators()
      {
          return iterators_;
      }

      const auto& iterators() const
      {
          return iterators_;
      }

    private:
      std::tuple<Iterators...> iterators_;
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
    using iterator       = zip_iterator<decltype( details::functor::begin ()( std::declval<Containers&>() ) )...>;
    using const_iterator = zip_iterator<decltype( details::functor::cbegin()( std::declval<Containers&>() ) )...>;

    using value_type = typename iterator::value_type;

    zip( Containers... containers )
      : containers_( std::forward<Containers>( containers )... )
    {}

    iterator begin()
    {
      return omp::tuple_map( details::functor::begin(), containers_);
    }

    iterator end()
    {
      return omp::tuple_map( details::functor::end(), containers_);
    }

    const_iterator begin() const
    {
      return omp::tuple_map( details::functor::cbegin(), containers_);
    }

    const_iterator end() const
    {
      return omp::tuple_map( details::functor::cend(), containers_);
    }

    const_iterator cbegin() const
    {
      return begin();
    }

    const_iterator cend() const
    {
      return end();
    }

    auto& containers()
    {
        return containers_;
    }

    const auto& containers() const
    {
        return containers_;
    }

  private:
    std::tuple<Containers...> containers_;
  };

  template<typename... Containers>
  zip( Containers&&... containers ) -> zip<Containers...>;
}
