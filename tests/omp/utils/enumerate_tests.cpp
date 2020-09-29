#include "omp/utils/enumerate.h"

#include "gtest/gtest.h"

#include <vector>


TEST( omp_enumerate, iterator_types )
{
    std::vector v = { 1, 2, 3, 4, 5 };

    auto e_is_iterator_1 = std::is_same_v<typename std::vector<int>::iterator,
        typename decltype( omp::enumerate( v ) )::container_iterator>;

    auto e_is_citerator_1 = std::is_same_v<typename std::vector<int>::const_iterator,
        typename decltype( omp::enumerate( std::as_const( v ) ) )::container_iterator>;

    auto e_is_iterator_2 = std::is_same_v<typename std::vector<int>::iterator,
        typename decltype( omp::enumerate( std::move( v ) ) )::container_iterator>;

    auto e_is_iterator_3 = std::is_same_v<typename std::vector<int>::iterator,
       typename decltype( omp::enumerate( std::move( std::as_const( v ) ) ) )::container_iterator>;

    ASSERT_TRUE( e_is_iterator_1 && e_is_citerator_1 && e_is_iterator_2 && e_is_iterator_3 );
}

TEST( omp_enumerate, iterator_for_loop_type )
{
  std::vector expecting = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  auto enumeration = omp::enumerate( expecting );

  for( auto it = std::begin( enumeration ); it != std::end( enumeration ); ++it )
    it->second += 1;

  for( auto it = std::begin( enumeration ); it != std::end( enumeration ); ++it )
    ASSERT_EQ( it->first + 1, it->second );
}

TEST( omp_enumerate, empty_case )
{
  std::vector<int> expecting = {};
  
  auto enumerated = omp::enumerate( expecting );
  
  ASSERT_TRUE( std::cbegin( enumerated ) == std::cend( enumerated ) );
}

TEST( omp_enumerate, values_from_zero_case )
{
  std::vector expecting = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  for( auto& [idx, value] : omp::enumerate( expecting ) )
    ASSERT_EQ( idx, value );
}

TEST( omp_enumerate, values_from_value_case )
{
  std::vector expecting = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

  for( auto& [idx, value] : omp::enumerate( expecting, 5 ) )
    ASSERT_EQ( idx, value );
}

TEST( omp_enumerate, moving_case )
{
  std::vector expecting = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  for( auto& [idx, value] : omp::enumerate( std::move( expecting ) ) )
    ASSERT_EQ( idx, value );
}

TEST( omp_enumerate, array_case )
{
  int expecting[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  for( auto& [idx, value] : omp::enumerate( expecting ) )
    ASSERT_EQ( idx, value );
}

TEST( omp_enumerate, pointers_case )
{
  int expecting[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  for( auto& [idx, value] : omp::enumerate( std::cbegin( expecting ), std::cend( expecting ) ) )
    ASSERT_EQ( idx, value );
}
