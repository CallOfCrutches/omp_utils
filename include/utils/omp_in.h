#pragma once

template<typename ValueType, typename ArgType1,typename ArgType2, typename... ArgTypes>
bool in( const ValueType& value, const ArgType1& arg1, const ArgType2& arg2, const ArgTypes&... args )
{
  return value == arg1 || value == arg2 || ( ( value == args ) || ... );
}

template<typename ValueType, typename ContainerType>
bool in( const ValueType& value, const ContainerType& container )
{
  return details::in_( value, container, nullptr );
}

namespace details
{
  template<typename ValueType, typename ContainerType>
  bool in_( const ValueType& value, const ContainerType& container,
            decltype( std::declval<ContainerType>().find( std::declval<ContainerType::key_type>() ) )* )
  {
    return container.find( value ) != std::cend( container );
  }

  template<typename ValueType, typename ContainerType>
  bool in_( const ValueType& value, const ContainerType& container, ... )
  {
    return std::find( std::cbegin( container ), std::cend( container ), value ) != std::cend( container );
  }
}