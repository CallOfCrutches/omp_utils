#pragma once
#include <xutility>


namespace utils
{
  namespace detail
  {

    template<typename Int>
    struct range_iterator
    {

      using value_type = Int;

      using iterator_category = std::bidirectional_iterator_tag;

      using reference = const value_type&;
      using pointer = const value_type*;

      range_iterator( value_type value, value_type step )
        : step( step )
        , value( value )
      {}


      reference operator*() const noexcept
      {
        return value;
      }

      pointer operator->() const noexcept
      {
        return &value;
      }

      range_iterator& operator++() noexcept
      {
        value += step;

        return *this;
      }

      range_iterator operator++( int ) noexcept
      {
        auto temp = *this;

        ++( *this );

        return temp;
      }

      range_iterator& operator--() noexcept
      {
        value -= step;

        return *this;
      }

      range_iterator operator--( int ) noexcept
      {
        auto temp = *this;

        --( *this );

        return temp;
      }

      bool operator==( const range_iterator& second ) const noexcept
      {
        return value == second.value;
      }

      bool operator!=( const range_iterator& second ) const noexcept
      {
        return !( *this == second );
      }

      bool operator<( const range_iterator& second ) const noexcept
      {
        return value < second.value;
      }

      bool operator>( const range_iterator& second ) const noexcept
      {
        return second < *this;
      }

      bool operator<=( const range_iterator& second ) const noexcept
      {
        return !( second < *this );
      }

      bool operator>=( const range_iterator& second ) const noexcept
      {
        return !( *this < second );
      }

    private:
      const value_type step;
      value_type value;
    };


    template<typename Int>
    struct range_container_adapter
    {
      using value_type = Int;

      using iterator = range_iterator<Int>;
      using const_iterator = iterator;

      range_container_adapter( value_type start, value_type stop, value_type step ) noexcept
        : start( start )
        , stop( stop )
        , step( step )
      {
        
      }

      const_iterator begin() const noexcept
      {
        return const_iterator( start, step );
      }

      const_iterator end() const noexcept
      {
        return const_iterator( stop, step );
      }

      bool empty() const noexcept
      {
        return start == stop;
      }

      std::size_t size() const noexcept
      {
        return static_cast<std::size_t>( ( stop - start ) / step );
      }

      const value_type start;
      const value_type stop;
      const value_type step;
    };

    inline std::int64_t align_stop( std::int64_t start, 
                             std::int64_t stop, 
                             std::int64_t step ) noexcept
    {
      if( start == stop )
        return start;

      if( start > stop && step > 0 )
        return start;

      if( start < stop && step < 0 )
        return start;

      auto mod = ( stop - start ) % step;

      if( mod == 0 )
        return stop;

      return stop + step - mod;
    }
  }

  inline auto range( std::int64_t start, std::int64_t stop, std::int64_t step = 1 )
  {
    if( step == 0 )
      throw std::logic_error( "step is zero" );

    stop = detail::align_stop( start, stop, step );

    return detail::range_container_adapter( start, stop, step );
  }

  inline auto range( std::int64_t stop )
  {
    return range( 0, stop );
  }
}
