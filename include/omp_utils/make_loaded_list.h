#pragma once

namespace utils
{
  template<typename T>
  struct get_record_by_index_ {};

  template<typename Ret, typename InterfaceType, typename IdxType, typename ValueType>
  struct get_record_by_index_<Ret( InterfaceType::* )( IdxType, ValueType )>
  {
    using result_type = Ret;
    using index_type  = IdxType;
    using value_type  = ValueType;
  };

#ifdef _ATL_VER
  template<typename ListType, typename IndexType, typename ValueType>
  class com_ptr_wrapper
  {

  public:
    using list_type  = ListType ;
    using value_type = ValueType;
    using size_type  = IndexType;

  public:
    com_ptr_wrapper( const CComPtr<list_type>& list )
      : list( list )
    {}

    list_type& get_list_() const noexcept
    {
      return *list;
    }

  private:
    CComPtr<list_type> list;
  };
#endif

#if 0
  template<typename ListType, typename IndexType, typename ValueType>
  class omp_ptr_wrapper
  {

  public:
    using list_type  = ListType ;
    using value_type = ValueType;
    using size_type  = IndexType;

  public:
    omp_ptr_wrapper( const COmpPtr<list_type>& list )
      : list( list )
    {}

    list_type& get_list_() const noexcept
    {
      return *list;
    }

  private:
    COmpPtr<list_type> list;
  };
#endif

  template<typename ListType, typename IndexType, typename ValueType>
  class shared_ptr_wrapper
  {

  public:
    using list_type  = ListType ;
    using value_type = ValueType;
    using size_type  = IndexType;

  public:
    shared_ptr_wrapper( const std::shared_ptr<list_type>& list )
      : list( list )
    {}

    list_type& get_list_() const noexcept
    {
      return *list;
    }

  private:
    std::shared_ptr<list_type> list;
  };


  template<typename ListWrapperType, typename IndexType, typename ValueType>
  class loaded_list_wrapper_iterator
  {

  public:
    using list_wrapper_type = ListWrapperType;

    using size_type  = IndexType;
    using value_type = ValueType;

    using iterator_category = std::random_access_iterator_tag;

    using difference_type = std::make_signed_t<size_type>;

    using reference = const value_type&;
    using pointer   = const value_type*;

  public:
    loaded_list_wrapper_iterator( const list_wrapper_type& list, const size_type index )
      : list( list )
    {
      advance_( index );
    }

    reference operator*() const noexcept
    {
      return value;
    }

    pointer operator->() const noexcept
    {
      return &value;
    }

    loaded_list_wrapper_iterator& operator++()
    {
      advance_( difference_type{ 1 } );
      return *this;
    }

    loaded_list_wrapper_iterator operator++( int )
    {
      auto ret = *this;
      ++( *this );

      return ret;
    }

    loaded_list_wrapper_iterator& operator--()
    {
      advance_( difference_type{ -1 } );
      return *this;
    }

    loaded_list_wrapper_iterator operator--( int )
    {
      auto ret = *this;
      --( *this );

      return ret;
    }

    loaded_list_wrapper_iterator& operator+=( const difference_type diff )
    {
      advance_( diff );
      return *this;
    }

    loaded_list_wrapper_iterator operator+( const difference_type diff ) const
    {
      auto ret = *this;

      return ret += diff;
    }

    loaded_list_wrapper_iterator& operator-=( const difference_type diff )
    {
      return *this += -diff;
    }

    loaded_list_wrapper_iterator operator-( const difference_type diff ) const
    {
      auto ret = *this;

      return ret -= diff;
    }

    difference_type operator-( const loaded_list_wrapper_iterator& rhs ) const noexcept
    {
      return static_cast<difference_type>( index ) - rhs.index;
    }

    reference operator[]( const difference_type diff ) const noexcept
    {
      return *( *this + diff );
    }

    bool operator==( const loaded_list_wrapper_iterator& rhs ) const noexcept
    {
      return index == rhs.index;
    }

    bool operator!=( const loaded_list_wrapper_iterator& rhs ) const noexcept
    {
      return !( *this == rhs );
    }

    bool operator<( const loaded_list_wrapper_iterator& rhs ) const noexcept
    {
      return index < rhs.index;
    }

    bool operator>( const loaded_list_wrapper_iterator& rhs ) const noexcept
    {
      return ( rhs < *this );
    }

    bool operator<=( const loaded_list_wrapper_iterator& rhs ) const noexcept
    {
      return !( *this > rhs );
    }

    bool operator>=( const loaded_list_wrapper_iterator& rhs ) const noexcept
    {
      return !( *this < rhs );
    }

  private:
    void advance_( const difference_type diff )
    {
      index += diff;

      if( index >= size_type{} && index < std::size( list ) )
        value = list[index];
    }

  private:
    const list_wrapper_type& list;

    size_type  index{};
    value_type value{};
  };

  template<typename InterfaceWrapperType>
  class loaded_list_wrapper: public InterfaceWrapperType
  {

  public:
    using base = InterfaceWrapperType;

    using list_type  = typename base::list_type ;
    using value_type = typename base::value_type;
    using size_type  = typename base::size_type ;

    using iterator = loaded_list_wrapper_iterator<loaded_list_wrapper, size_type, value_type>;
    using const_iterator = iterator;

  public:

    template<typename U>
    loaded_list_wrapper( const U& ptr ): InterfaceWrapperType( ptr ) 
    {
      get_list_().Count( &count );
    }

    value_type at( const size_type idx ) const
    {
      if( idx < size_type{} || idx >= count )
        throw std::out_of_range( "invalid loaded_list_wrapper index" );

      return ( *this )[idx];
    }

    value_type operator[]( const size_type idx ) const
    {
      value_type value{};

      base::get_list_().GetRecordByIndex( idx, &value );

      return value;
    }

    value_type front() const
    {
      return ( *this )[size_type{}];
    }

    const_iterator begin() const
    {
      return const_iterator( *this, size_type{} );
    }

    const_iterator end() const
    {
      return const_iterator( *this, std::size( *this ) );
    }

    const_iterator cbegin() const
    {
      return begin();
    }

    const_iterator cend() const
    {
      return end();
    }

    bool empty() const noexcept
    {
      return count == size_type{};
    }

    size_type size() const noexcept
    {
      return count;
    }

  private:
    size_type count{};
  };
  
#if 0
  template<typename T, typename... Args>
  auto make_loaded_list( COmpPtr<T>& list, Args&&... args )
  {
    using record_by_index = get_record_by_index_<decltype( &T::GetRecordByIndex )>;

    using value_type = std::remove_pointer_t<record_by_index::value_type>;

    list->LoadDataList( std::forward<Args>( args )... );

    return loaded_list_wrapper<omp_ptr_wrapper<T, record_by_index::index_type, value_type>>( list );
  }
#endif

ifdef _ATL_VER
  template<typename T, typename... Args>
  auto make_loaded_list( CComPtr<T>& list, Args&&... args )
  {
    using record_by_index = get_record_by_index_<decltype( &T::GetRecordByIndex )>;

    using value_type = std::remove_pointer_t<record_by_index::value_type>;

    list->LoadDataList( std::forward<Args>( args )... );

    return loaded_list_wrapper<com_ptr_wrapper<T, record_by_index::index_type, value_type>>( list );
  }
#endif

  template<typename T, typename... Args>
  auto make_loaded_list( std::shared_ptr<T>& list, Args&&... args )
  {
    using record_by_index = get_record_by_index_<decltype( &T::GetRecordByIndex )>;

    using value_type = std::remove_pointer_t<record_by_index::value_type>;

    list->LoadDataList( std::forward<Args>( args )... );

    return loaded_list_wrapper<shared_ptr_wrapper<T, record_by_index::index_type, value_type>>( list );
  }
}
//TODO: add operator[] method
