#include "gtest/gtest.h"
#include "omp_utils/omp_reversed.h"
#include <vector>
#include <list>


TEST( utils_reversed, reference_case )
{
  std::vector values = { 1,4,3,2,5 };

  auto reversed_values = utils::reversed( values );

  std::vector expecting = { 5,2,3,4,1 };

  ASSERT_TRUE( std::equal( std::cbegin( reversed_values ), std::cend( reversed_values ),
              std::cbegin( expecting ), std::cend( expecting ) ) );
}

TEST( utils_reversed, move_case )
{
  std::vector values = { 1,4,3,2,5 };

  auto reversed_values = utils::reversed( std::move( values ) );

  std::vector expecting = { 5,2,3,4,1 };

  ASSERT_TRUE( std::equal( std::cbegin( reversed_values ), std::cend( reversed_values ),
                           std::cbegin( expecting ), std::cend( expecting ) ) );
}

TEST( utils_reversed, const_reference_case )
{
  const std::vector values = { 1,4,3,2,5 };

  auto reversed_values = utils::reversed( values );

  std::vector expecting = { 5,2,3,4,1 };

  ASSERT_TRUE( std::equal( std::cbegin( reversed_values ), std::cend( reversed_values ),
                           std::cbegin( expecting ), std::cend( expecting ) ) );
}

TEST( utils_reversed, const_move_case )
{
  const std::vector values = { 1,4,3,2,5 };

  auto reversed_values = utils::reversed( std::move( values ) );

  std::vector expecting = { 5,2,3,4,1 };

  ASSERT_TRUE( std::equal( std::cbegin( reversed_values ), std::cend( reversed_values ),
                           std::cbegin( expecting ), std::cend( expecting ) ) );
}

TEST( utils_reversed, reference_case_increase_values )
{
  std::list values = { 1,4,3,2,5 };

  auto reversed_values = utils::reversed( values );

  std::list expecting = { 6,3,4,5,2 };

  for( auto& v : reversed_values )
    ++v;

  ASSERT_TRUE( std::equal( std::cbegin( reversed_values ), std::cend( reversed_values ),
                           std::cbegin( expecting ), std::cend( expecting ) ) );
}

TEST( utils_reversed, move_case_increase_values )
{
  std::list values = { 1,4,3,2,5 };

  auto reversed_values = utils::reversed( std::move( values ) );

  std::list expecting = { 6,3,4,5,2 };

  for( auto& v : reversed_values )
    ++v;

  ASSERT_TRUE( std::equal( std::cbegin( reversed_values ), std::cend( reversed_values ),
                           std::cbegin( expecting ), std::cend( expecting ) ) );
}

TEST( utils_reversed, reference_rereverse_case )
{
  std::list values = { 1,4,3,2,5 };

  auto rereversed_values = utils::reversed( utils::reversed( values ) );

  ASSERT_TRUE( std::equal( std::cbegin( rereversed_values ), std::cend( rereversed_values ),
                           std::cbegin( values ), std::cend( values ) ) );
}

TEST( utils_reversed, move_rereverse_case )
{
  std::list values = { 1,4,3,2,5 };

  auto rereversed_values = utils::reversed( utils::reversed( std::move( values ) ) );

  ASSERT_FALSE( std::equal( std::cbegin( rereversed_values ), std::cend( rereversed_values ),
                           std::cbegin( values ), std::cend( values ) ) );
}

TEST( utils_reversed, reference_rerereverse_case )
{
  std::list values = { 1,4,3,2,5 };

  auto rereversed_values = utils::reversed( utils::reversed( utils::reversed( values ) ) );

  ASSERT_TRUE( std::equal( std::cbegin( rereversed_values ), std::cend( rereversed_values ),
                           std::crbegin( values ), std::crend( values ) ) );
}

TEST( utils_reversed, reference_reverse_case )
{
  std::vector values = { 1,4,3,2,5 };

  auto reversed_values = utils::reversed( values );

  ASSERT_TRUE( std::equal( std::crbegin( reversed_values ), std::crend( reversed_values ),
                           std::cbegin( values ), std::cend( values ) ) );
}
