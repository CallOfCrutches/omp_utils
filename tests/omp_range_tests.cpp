#include "gtest/gtest.h"
#include "omp_utils/omp_range.h"


TEST( omp_range, zero_one_overload )
{
  auto r = omp::range( 0 );

  ASSERT_EQ( std::cbegin( r ), std::cend( r ) );
}

TEST( omp_range, zero_three_overload )
{
  auto r = omp::range( 0, 0 );

  ASSERT_EQ( std::cbegin( r ), std::cend( r ) );
}

TEST( omp_range, step_zero )
{
  ASSERT_THROW( omp::range( 0, 10, 0 ), std::logic_error );
}

TEST( omp_range, out_of_range_one_overload )
{
  auto r = omp::range( -10 );

  ASSERT_EQ( std::cbegin( r ), std::cend( r ) );
}

TEST( omp_range, out_of_range_three_overload )
{
  auto r = omp::range( 10, 5 );

  ASSERT_EQ( std::cbegin( r ), std::cend( r ) );
}

TEST( omp_range, incorrect_step_three_overload )
{
  auto r = omp::range( 5, 10, -1 );

  ASSERT_EQ( std::cbegin( r ), std::cend( r ) );
}

TEST( omp_range, simple_case_one_overload )
{
  const std::vector<std::int64_t> expecting = { 0,1,2,3,4 };

  std::vector<std::int64_t> result;

  for( auto v : omp::range( 5 ) )
    result.emplace_back( v );

  ASSERT_EQ( result, expecting );
}

TEST( omp_range, simple_case_three_overload )
{
  const std::vector<std::int64_t> expecting = { 1,2,3,4,5 };

  std::vector<std::int64_t> result;

  for( auto v : omp::range( 1, 6 ) )
    result.emplace_back( v );

  ASSERT_EQ( result, expecting );
}

TEST( omp_range, simple_case_with_step_increase )
{
  const std::vector<std::int64_t> expecting = { 0, 2, 4, 6, 8, 10 };

  std::vector<std::int64_t> result;

  for( auto v : omp::range( 0, 12, 2 ) )
    result.emplace_back( v );

  ASSERT_EQ( result, expecting );
}

TEST( omp_range, simple_case_with_step_decrease )
{
  const std::vector<std::int64_t> expecting = { 10, 8, 6, 4, 2, 0 };

  std::vector<std::int64_t> result;

  for( auto v : omp::range( 10, -2, -2 ) )
    result.emplace_back( v );

  ASSERT_EQ( result, expecting );
}

TEST( omp_range, simple_case_with_step_increase_negative )
{
  const std::vector<std::int64_t> expecting = { -16, -14, -12, -10 ,-8 };

  std::vector<std::int64_t> result;

  for( auto v : omp::range( -16, -6, 2 ) )
    result.emplace_back( v );

  ASSERT_EQ( result, expecting );
}

TEST( omp_range, simple_case_with_step_decrease_negative )
{
  const std::vector<std::int64_t> expecting = { -6, -8, -10, -12 ,-14 };

  std::vector<std::int64_t> result;

  for( auto v : omp::range( -6, -16, -2 ) )
    result.emplace_back( v );

  ASSERT_EQ( result, expecting );
}

TEST( omp_range, special_case_with_step_increase )
{
  const std::vector<std::int64_t> expecting = { 1, 5, 9, };

  std::vector<std::int64_t> result;

  for( auto v : omp::range( 1, 12, 4 ) )
    result.emplace_back( v );

  ASSERT_EQ( result, expecting );
}

TEST( omp_range, special_case_with_step_decrease )
{
  const std::vector<std::int64_t> expecting = { 21, 16, 11, };

  std::vector<std::int64_t> result;

  for( auto v : omp::range( 21, 7, -5 ) )
    result.emplace_back( v );

  ASSERT_EQ( result, expecting );
}

TEST( omp_range, special_case_with_step_increase_negative )
{
  const std::vector<std::int64_t> expecting = { -41, -35, -29, -23, -17, };

  std::vector<std::int64_t> result;

  for( auto v : omp::range( -41, -12, 6 ) )
    result.emplace_back( v );

  ASSERT_EQ( result, expecting );
}

TEST( omp_range, special_case_with_step_decrease_negative )
{
  const std::vector<std::int64_t> expecting = { -6, -14, -22, -30, -38, };

  std::vector<std::int64_t> result;

  for( auto v : omp::range( -6, -44, -8 ) )
    result.emplace_back( v );

  ASSERT_EQ( result, expecting );
}

TEST( omp_range, simple_case_with_step_increase_both )
{
  const std::vector<std::int64_t> expecting = { -47, -31, -15, 1, 17, 33, 49, 65 };

  std::vector<std::int64_t> result;

  for( auto v : omp::range( -47, 81, 16 ) )
    result.emplace_back( v );

  ASSERT_EQ( result, expecting );
}

TEST( omp_range, simple_case_with_step_decrease_both )
{
  const std::vector<std::int64_t> expecting = { 81, 65, 49, 33, 17, 1, -15, -31, };

  std::vector<std::int64_t> result;

  for( auto v : omp::range( 81, -47, -16 ) )
    result.emplace_back( v );

  ASSERT_EQ( result, expecting );
}

TEST( omp_range, special_case_with_step_increase_both )
{
  const std::vector<std::int64_t> expecting = { -53, -40, -27, -14, -1, 12, 25, 38, 51, 64, 77 };

  std::vector<std::int64_t> result;

  for( auto v : omp::range( -53, 89, 13 ) )
    result.emplace_back( v );

  ASSERT_EQ( result, expecting );
}

TEST( omp_range, special_case_with_step_decrease_both )
{
  const std::vector<std::int64_t> expecting = { 91, 70, 49, 28, 7, -14, -35 };

  std::vector<std::int64_t> result;

  for( auto v : omp::range( 91, -49, -21 ) )
    result.emplace_back( v );

  ASSERT_EQ( result, expecting );
}
