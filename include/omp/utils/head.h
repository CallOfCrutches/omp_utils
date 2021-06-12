#pragma once

#include <cstddef>
#include <iterator>


namespace omp
{
  namespace details
  {
    template<typename Iterator>
    struct head_iterator
    {
      using iterator_category = std::forward_iterator_tag;
      using value_type = typename std::iterator_traits<Iterator>::value_type;
      using reference = typename std::iterator_traits<Iterator>::reference;
      using pointer = typename std::iterator_traits<Iterator>::pointer;

      head_iterator( Iterator iterator, std::size_t limit )
        : _iterator( iterator )
        , _limit( limit )
      {}

      reference operator*() const noexcept( noexcept( std::declval<Iterator>().operator*() ) )
      {
        return *_iterator;
      }

      pointer operator->() const noexcept( noexcept( std::declval<Iterator>().operator->() ) )
      {
        return _iterator;
      }

      head_iterator& operator++() noexcept( noexcept( std::declval<Iterator>().operator++() ) )
      {
        --_limit;
        ++_iterator;
        return *this;
      }

      head_iterator operator++( int ) noexcept( noexcept( std::declval<head_iterator>().operator++() ) )
      {
        auto old = *this;
        ++( *this );
        return old;
      }

      bool operator==( const head_iterator& rhs ) const
        noexcept( noexcept( std::declval<Iterator>().operator==(std::declval<Iterator>() ) ) )
      {
        return _limit == rhs._limit || _iterator == rhs._iterator;
      }

      bool operator!=( const head_iterator& rhs ) const
        noexcept( noexcept( std::declval<head_iterator>().operator==(std::declval<head_iterator>() ) ) )
      {
        return !(*this == rhs);
      }

    private:
      Iterator _iterator;
      std::size_t _limit;
    };
  }


  template<typename Container>
  struct head
  {
    using decayed_container = std::decay_t<Container>;

    using iterator       = details::head_iterator<typename decayed_container::iterator>;
    using const_iterator = details::head_iterator<typename decayed_container::const_iterator>;

    using value_type = typename decayed_container::value_type;

    head( Container&& container, std::size_t limit )
      : _container( std::forward<Container>( container ) )
      , _limit( limit )
    {}

    iterator begin() noexcept( noexcept( std::declval<decayed_container>().begin() ) )
    {
      return iterator( _container.begin(), _limit );
    }

    iterator end() noexcept( noexcept( std::declval<decayed_container>().end() ) )
    {
      return iterator( _container.end(), std::size_t{} );
    }

    const_iterator begin() const noexcept( noexcept( std::declval<std::add_const_t<decayed_container>>().begin() ) )
    {
      return const_iterator( _container.begin(), _limit );
    }

    const_iterator end() const noexcept( noexcept( std::declval<std::add_const_t<decayed_container>>().end() ) )
    {
      return const_iterator( _container.end(), std::size_t{} );
    }

    auto cbegin() const noexcept( noexcept( std::declval<std::add_const_t<head>>().begin() ) )
    {
      return begin();
    }

    auto cend() const noexcept( noexcept( std::declval<std::add_const_t<head>>().end() ) )
    {
      return end();
    }

    bool empty() const noexcept
    {
      using std::empty;
      return ::empty( _container );
    }

    std::size_t size() const noexcept
    {
      using std::size;
      return std::min( ::size( _container ), _limit );
    }

  private:
    Container _container;
    std::size_t _limit;
  };

  template<typename Container>
  head( Container&&, std::size_t )->head<Container>;
}
