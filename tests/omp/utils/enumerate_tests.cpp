#include "omp/utils/enumerate.h"

#include "gtest/gtest.h"

#include <vector>


TEST( omp_enumerate, iterator_case )
{
  std::vector expecting = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  auto enumerated = omp::enumerate( expecting );

  ASSERT_EQ( ( ++std::cbegin( enumerated ) )->second, 1 );

  {
    auto it = ++std::cbegin( enumerated );
    ++it;
    ++it;

    ASSERT_EQ( it->second, 3 );
  }
}

TEST( omp_enumerate, iterator_comparisons_case )
{
  std::vector expecting = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  auto enumerated = omp::enumerate( expecting );

  ASSERT_NE( std::cbegin( enumerated ), std::cend( enumerated )   );
  ASSERT_LE( std::cbegin( enumerated ), std::cend( enumerated )   );
  ASSERT_GE( std::cend( enumerated )  , std::cbegin( enumerated ) );
  ASSERT_EQ( ( ++std::cbegin( enumerated ) ), ( ++std::cbegin( enumerated ) ) );

  {
    auto it2 = ++std::cbegin( enumerated );
    auto it1 = it2++;

    ASSERT_LE( it1, it2 );
  }
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

  for( auto&[idx, value] : omp::enumerate( expecting, 5 ) )
    ASSERT_EQ( idx, value );
}

TEST( omp_enumerate, increasing_value )
{
  std::vector initial = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  for( auto& [idx, value] : omp::enumerate( initial, 5 ) )
    value += idx;

  std::vector expecting = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

  auto iIt = std::cbegin( initial );
  auto eIt = std::cbegin( expecting );

  ASSERT_TRUE( std::equal( std::cbegin( initial ), std::cend( initial ),
                           std::cbegin( expecting ), std::cend( expecting ) ) );
}

TEST( omp_enumerate, moving_case )
{
  std::vector expecting = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  for( auto&[idx, value] : omp::enumerate( std::move( expecting ) ) )
    ASSERT_EQ( idx, value );
}

TEST( omp_enumerate, array_case )
{
  int expecting[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  for( auto&[idx, value] : omp::enumerate( expecting ) )
    ASSERT_EQ( idx, value );
}

TEST( omp_enumerate, pointers_case )
{
  int expecting[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  for( auto&[idx, value] : omp::enumerate( std::cbegin( expecting ), std::cend( expecting) ) )
    ASSERT_EQ( idx, value );
}
