#pragma once

#include <cstdint>
#include <iterator>
#include <exception>


namespace omp
{
  namespace details
  {

    template<typename Integral>
    struct range_iterator
    {
      using iterator_category = std::forward_iterator_tag;

      using value_type = Integral;

      using reference = const value_type&;
      using pointer = const value_type*;

      using difference_type = value_type;

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


    template<typename Integral>
    struct range
    {
      using value_type = Integral;

      using iterator       = range_iterator<value_type>;
      using const_iterator = range_iterator<value_type>;

      range( value_type start, value_type stop, value_type step ) noexcept
        : start_( start )
        , stop_( stop )
        , step_( step )
      {}

      const_iterator begin() const noexcept
      {
        return const_iterator( start_, step_ );
      }

      const_iterator end() const noexcept
      {
        return const_iterator( stop_, step_ );
      }

      bool empty() const noexcept
      {
        return start_ == stop_;
      }

      std::size_t size() const noexcept
      {
        return static_cast<std::size_t>( ( stop_ - start_ ) / step_ );
      }

    private:
      const value_type start_;
      const value_type stop_;
      const value_type step_;
    };

    template<typename Integral>
    inline Integral align_stop( Integral start, Integral stop, Integral step ) noexcept
    {
      if( start == stop )
        return start;

      if( start > stop && step > 0 )
        return start;

      if( start < stop && step < 0 )
        return start;

      auto mod = ( stop - start ) % step;

      return mod != 0 ? stop + step - mod : stop;
    }
  }

  template<typename Start, typename Stop, typename Step = std::common_type_t<Start, Stop>>
  inline auto range( Start start, Stop stop, Step step = Step{1} )
  {
    using Integral = std::common_type_t<Start, Stop, Step>;

    static_assert( std::is_integral_v<Integral>, "Type is not integral" );

    if( !step ) throw std::logic_error( "step is zero" );

    stop = details::align_stop<Integral>( start, stop, step );
    return details::range<Integral>( start, stop, step );
  }

  template<typename Integral>
  inline auto range( Integral stop )
  {
    return range( Integral{0}, stop );
  }
}
