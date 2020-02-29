#include "omp/utils/reversed.h"

#include "gtest/gtest.h"

#include <vector>
#include <list>


TEST( omp_reversed, reference_case )
{
  std::vector values = { 1,4,3,2,5 };

  auto reversed_values = omp::reversed( values );

  std::vector expecting = { 5,2,3,4,1 };

  ASSERT_TRUE( std::equal( std::cbegin( reversed_values ), std::cend( reversed_values ),
              std::cbegin( expecting ), std::cend( expecting ) ) );
}

TEST( omp_reversed, move_case )
{
  std::vector values = { 1,4,3,2,5 };

  auto reversed_values = omp::reversed( std::move( values ) );

  std::vector expecting = { 5,2,3,4,1 };

  ASSERT_TRUE( std::equal( std::cbegin( reversed_values ), std::cend( reversed_values ),
                           std::cbegin( expecting ), std::cend( expecting ) ) );
}

TEST( omp_reversed, const_reference_case )
{
  const std::vector values = { 1,4,3,2,5 };

  auto reversed_values = omp::reversed( values );

  std::vector expecting = { 5,2,3,4,1 };

  ASSERT_TRUE( std::equal( std::cbegin( reversed_values ), std::cend( reversed_values ),
                           std::cbegin( expecting ), std::cend( expecting ) ) );
}

TEST( omp_reversed, const_move_case )
{
  const std::vector values = { 1,4,3,2,5 };

  auto reversed_values = omp::reversed( std::move( values ) );

  std::vector expecting = { 5,2,3,4,1 };

  ASSERT_TRUE( std::equal( std::cbegin( reversed_values ), std::cend( reversed_values ),
                           std::cbegin( expecting ), std::cend( expecting ) ) );
}

TEST( omp_reversed, reference_case_increase_values )
{
  std::list values = { 1,4,3,2,5 };

  auto reversed_values = omp::reversed( values );

  std::list expecting = { 6,3,4,5,2 };

  for( auto& v : reversed_values )
    ++v;

  ASSERT_TRUE( std::equal( std::cbegin( reversed_values ), std::cend( reversed_values ),
                           std::cbegin( expecting ), std::cend( expecting ) ) );
}

TEST( omp_reversed, move_case_increase_values )
{
  std::list values = { 1,4,3,2,5 };

  auto reversed_values = omp::reversed( std::move( values ) );

  std::list expecting = { 6,3,4,5,2 };

  for( auto& v : reversed_values )
    ++v;

  ASSERT_TRUE( std::equal( std::cbegin( reversed_values ), std::cend( reversed_values ),
                           std::cbegin( expecting ), std::cend( expecting ) ) );
}
