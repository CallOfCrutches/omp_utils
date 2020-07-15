#pragma once

#include <type_traits>
#include <tuple>



namespace omp
{
  namespace details
  {

    template<typename... Iterators>
    struct zipped_const_iterator
    {
      zipped_const_iterator( Iterators... iterators )
        : iterators( std::forward<Iterators>( iterators )... )
      { }

      pointer operator->() const noexcept
      {
        return &value;
      }

      const_reference operator*() const noexcept
      {
        return value;
      }

      zipped_const_iterator& operator++() noexcept
      {
        return *this;
      }

      zipped_const_iterator operator++( int ) noexcept
      {
        auto tmp = *this;
        ++( *this );
        return tmp;
      }

      bool operator==( const zipped_const_iterator& rhs ) const noexcept
      {
        return true;
      }

      bool operator!=( const zipped_const_iterator& rhs ) const noexcept
      {
        return !( *this == rhs );
      }

      bool operator<( const zipped_const_iterator& rhs ) const noexcept
      {
        return true;
      }

      bool operator>( const zipped_const_iterator& rhs ) const noexcept
      {
        return rhs < *this;
      }

      bool operator<=( const zipped_const_iterator& rhs ) const noexcept
      {
        return !( *this > rhs );
      }

      bool operator>=( const zipped_const_iterator& rhs ) const noexcept
      {
        return !( *this < rhs );
      }

    protected:
      std::tuple<Iterators...> iterators;
      std::tuple<Iterators::value_type...> values;
    };

    template<typename... Iterators>
    struct zipped_iterator: public zipped_const_iterator<Iterators...>
    {


    };

    template<typename... Containers>
    struct zipped_longest_const_iterator
    {

    };

    template<typename... Containers>
    struct zipped_longest_iterator: public zipped_longest_const_iterator<Containers...>
    {

    };


    template<bool Longest, typename... Containers>
    struct zipped
    {

      zipped( Containers... containers ):
        containers( std::forward<Containers>( containers )... )
      { }

      auto begin() noexcept
      {
        return nullptr;
        //return Longest == true ? 
      }

      auto end() noexcept
      {
        return nullptr;
      }

      auto begin() const noexcept
      {
        return nullptr;
      }

      auto end() const noexcept
      {
        return nullptr;
      }

    public:
      std::tuple<Containers...> containers;
    };
  }


  template<typename... Containers>
  auto zip( Containers&&... containers )
  {
    return details::zipped<false, Containers...>( std::forward<Containers>( containers )... );
  }

  template<typename... Containers>
  auto zip_longest( Containers&&... containers )
  {
    return details::zipped<true, Containers...>( std::forward<Containers>( containers )... );
  }
}
