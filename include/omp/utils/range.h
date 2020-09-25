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
        : value_( value )
        , step_( step )
      {}

      reference operator*() const noexcept
      {
        return value_;
      }

      pointer operator->() const noexcept
      {
        return &value_;
      }

      range_iterator& operator++() noexcept
      {
        value_ += step_;
        return *this;
      }

      range_iterator operator++( int ) noexcept
      {
        auto temp = *this;
        ++( *this );

        return temp;
      }

      bool operator==( const range_iterator& rhs ) const noexcept
      {
        return step_ != rhs.step_ || compare( value_, rhs.value_, step_ );
      }

      bool operator!=( const range_iterator& rhs ) const noexcept
      {
        return !( *this == rhs );
      }

    private:
      bool compare( value_type lhs, value_type rhs, value_type sign ) const noexcept
      {
          return sign > 0 ? lhs >= rhs : lhs <= rhs;
      }

    private:
      const value_type step_;
      value_type value_;
    };
  }

  template<typename Integral>
  struct range
  {
    using value_type = Integral;

    using iterator       = details::range_iterator<value_type>;
    using const_iterator = details::range_iterator<value_type>;

    static_assert( std::is_integral_v<value_type>, "Type is not integral" );

    range( value_type stop )
      : range( value_type{ 0 }, stop, value_type{ 1 } )
    {}

    range( value_type start, value_type stop )
      : range( start, stop, value_type{ 1 } )
    {}

    range( value_type start, value_type stop, value_type step )
      : start_( start )
      , stop_( stop )
      , step_( step )
    {
      if( !step_ )
        throw std::logic_error("step is zero");
    }

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

  template<typename Int>
  range( Int ) -> range<Int>;

  template<typename Int1, typename Int2>
  range( Int1, Int2 ) -> range<std::common_type_t<Int1, Int2>>;

  template<typename Int1, typename Int2, typename Int3>
  range( Int1, Int2, Int3 ) -> range<std::common_type_t<Int1, Int2, Int3>>;

}
