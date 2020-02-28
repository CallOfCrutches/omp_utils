#pragma once
#include <optional>
#include <type_traits>


namespace omp
{
  namespace details
  {
    template<typename ContainerType>
    struct enumerate_ref_
    {
      using container_type = ContainerType;

      enumerate_ref_( container_type& container )
        : container_( container )
      { }

      auto& container() noexcept
      {
        return container_;
      }

      const auto& container() const noexcept
      {
        return container_;
      }

      container_type& container_;
    };

    template<typename ContainerType>
    struct enumerate_value_
    {
      using container_type = const ContainerType;

      enumerate_value_( std::remove_cv_t<container_type>&& container )
        : container_( std::move( container ) )
      { }

      const auto& container() const noexcept
      {
        return container_;
      }

      container_type container_;
    };

    template<typename ValueType>
    struct enumerate_ptr_
    {
      using container_type = enumerate_ptr_;

      using iterator       = ValueType*;
      using const_iterator = const ValueType*;

      enumerate_ptr_( ValueType* begin, ValueType* end )
        : begin_( begin )
        , end_( end )
      { }

      auto& container() noexcept
      {
        return *this;
      }

      const auto& container() const noexcept
      {
        return *this;
      }

      ValueType* begin() noexcept
      {
        return begin_;
      }

      ValueType* end() noexcept
      {
        return end_;
      }

      const ValueType* begin() const noexcept
      {
        return begin_;
      }

      const ValueType* end() const noexcept
      {
        return end_;
      }

      ValueType* begin_{};
      ValueType* end_{};
    };
  }

  template<typename Iterator>
  struct const_enumerate_iterator
  {
    using iterator_category = std::bidirectional_iterator_tag;

    using wrapped_iterator = Iterator;

    using value_type = std::pair<const std::ptrdiff_t, typename const std::iterator_traits<wrapped_iterator>::reference>;

    using pointer   = const value_type*;
    using reference = const value_type&;

    const_enumerate_iterator( wrapped_iterator iter, std::ptrdiff_t value )
      : iterator_( std::move( iter ) )
      , value_( value )
    { }

    reference operator*() noexcept
    {
      return *get_value_();
    }

    pointer operator->() noexcept
    {
      return &*get_value_();
    }

    const_enumerate_iterator& operator++()
    {
      ++iterator_;
      ++value_;

      return *this;
    }

    const_enumerate_iterator& operator--()
    {
      --iterator_;
      --value_;

      return *this;
    }

    const_enumerate_iterator operator++( int )
    {
      auto tmp = *this;

      ++( *this );

      return tmp;
    }

    const_enumerate_iterator operator--( int )
    {
      auto tmp = *this;

      --( *this );

      return tmp;
    }


    bool operator<( const wrapped_iterator& rhs ) noexcept
    {
      return iterator_ < rhs;
    }

    bool operator>( const wrapped_iterator& rhs ) noexcept
    {
      return iterator_ > rhs;
    }

    bool operator<=( const wrapped_iterator& rhs ) noexcept
    {
      return iterator_ <= rhs;
    }

    bool operator>=( const wrapped_iterator& rhs ) noexcept
    {
      return iterator_ >= rhs;
    }

    bool operator==( const wrapped_iterator& rhs ) noexcept
    {
      return iterator_ == rhs;
    }

    bool operator!=( const wrapped_iterator& rhs ) noexcept
    {
      return iterator_ != rhs;
    }

    std::optional<value_type>& get_value_()
    {
      if( !pair_value_.has_value() || pair_value_->first != value_ )
        pair_value_.emplace( value_, *iterator_ );

      return pair_value_;
    }

    wrapped_iterator iterator_;
    std::ptrdiff_t value_;
    std::optional<value_type> pair_value_;
  };

  template<typename Iterator>
  struct enumerate_iterator
  {
    using iterator_category = std::bidirectional_iterator_tag;

    using wrapped_iterator = Iterator;

    using value_type = std::pair<const std::ptrdiff_t, typename std::iterator_traits<wrapped_iterator>::reference>;

    using pointer   = value_type*;
    using reference = value_type&;

    enumerate_iterator( wrapped_iterator iter, std::ptrdiff_t value )
      : iterator_( std::move( iter ) )
      , value_( value )
    { }

    reference operator*() noexcept
    {
      return *get_value_();
    }

    pointer operator->() noexcept
    {
      return &*get_value_();
    }

    enumerate_iterator& operator++()
    {
      ++iterator_;
      ++value_;

      return *this;
    }

    enumerate_iterator& operator--()
    {
      --iterator_;
      --value_;

      return *this;
    }

    enumerate_iterator operator++( int )
    {
      auto tmp = *this;

      ++( *this );

      return tmp;
    }

    enumerate_iterator operator--( int )
    {
      auto tmp = *this;

      --( *this );

      return tmp;
    }

    bool operator<( const wrapped_iterator& rhs ) noexcept
    {
      return iterator_ < rhs;
    }

    bool operator>( const wrapped_iterator& rhs ) noexcept
    {
      return iterator_ > rhs;
    }

    bool operator<=( const wrapped_iterator& rhs ) noexcept
    {
      return iterator_ <= rhs;
    }

    bool operator>=( const wrapped_iterator& rhs ) noexcept
    {
      return iterator_ >= rhs;
    }

    bool operator==( const wrapped_iterator& rhs ) noexcept
    {
      return iterator_ == rhs;
    }

    bool operator!=( const wrapped_iterator& rhs ) noexcept
    {
      return iterator_ != rhs;
    }

    std::optional<value_type>& get_value_()
    {
      if( !pair_value_.has_value() || pair_value_->first != value_ )
        pair_value_.emplace( value_, *iterator_ );

      return pair_value_;
    }

    wrapped_iterator iterator_;
    std::ptrdiff_t value_;
    std::optional<value_type> pair_value_;
  };

  template<typename EnumeratorBase>
  struct enumerator: public EnumeratorBase
  {
    using base_type = EnumeratorBase;

    using container_type = typename base_type::container_type;

    using container_iterator = std::conditional_t<!std::is_const_v<container_type>, 
                                                   typename container_type::iterator,
                                                   typename container_type::const_iterator>;

    using container_const_iterator = typename container_type::const_iterator;

    using iterator       = enumerate_iterator<container_iterator>;
    using const_iterator = const_enumerate_iterator<container_const_iterator>;

    template<typename container_type = container_type>
    enumerator( container_type&& container, std::ptrdiff_t start )
      : base_type( std::forward<container_type>( container ) )
      , start_( start )
    { }

    iterator begin() noexcept
    {
      return iterator( base_type::container().begin(), start_ );
    }

    container_iterator end() noexcept
    {
      return base_type::container().end();
    }

    const_iterator begin() const noexcept
    {
      return const_iterator( base_type::container().begin(), start_ );
    }

    container_const_iterator end() const noexcept
    {
      return base_type::container().end();
    }
    
    const_iterator cbegin() const noexcept
    {
      return begin();
    }
    
    container_const_iterator cend() const noexcept
    {
      return end();
    }

    std::ptrdiff_t start_{};
  };

  template<typename ContainerType>
  enumerator( ContainerType&&, std::ptrdiff_t )->enumerator<details::enumerate_value_<ContainerType>>;

  template<typename ContainerType>
  enumerator( ContainerType&, std::ptrdiff_t )->enumerator<details::enumerate_ref_<ContainerType>>;

  template<typename ValueType>
  enumerator( details::enumerate_ptr_<ValueType>&&, std::ptrdiff_t )->enumerator<details::enumerate_ptr_<ValueType>>;

  template<typename ContainerType>
  auto enumerate( ContainerType&& container, std::ptrdiff_t start = 0 )
  {
    return enumerator( std::forward<ContainerType>( container ), start );
  }

  template<typename ValueType, size_t size>
  auto enumerate( ValueType( &arr )[size], std::ptrdiff_t start = 0 )
  {
    return enumerator( details::enumerate_ptr_( arr, arr + size ), start );
  }

  template<typename ValueType>
  auto enumerate( ValueType* begin, ValueType* end, std::ptrdiff_t start = 0 )
  {
    return enumerator( details::enumerate_ptr_( begin, end ), start );
  }
}

//TODO: fix reversed + enumerate
