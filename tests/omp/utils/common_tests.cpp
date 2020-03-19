#include "omp/utils/common.h"

#include "gtest/gtest.h"

namespace
{
  struct test_element
  {
    test_element( int value ) noexcept
      : value( value )
    { }

    bool operator<( const test_element element ) const noexcept
    {
      return value < element.value;
    }

    bool operator==( const test_element element ) const noexcept
    {
      return value == element.value;
    }

    test_element operator+( const test_element element ) const noexcept
    {
      return value + element.value;
    }

    bool operator>( const test_element element ) const noexcept
    {
      return element < *this;
    }

    int value;
  };
}

TEST( omp_min, primitive_types )
{
  ASSERT_EQ( omp::min( 2, 55, 6, 34, -2, 54, 43 ), -2 );

  ASSERT_EQ( omp::min( 1, 1, 1, 1, 1, 1, 1, 1 ), 1 );

  ASSERT_EQ( omp::min( 1, 12, 5, 1, 7, 9, 4, 0 ), 0 );

  ASSERT_EQ( omp::min( 1, 12, 5, 21, 7, 9, 4, 10 ), 1 );

  ASSERT_EQ( omp::min( 1.5, 12, 5, 21, 7, 9, 4, 1.2 ), 1.2 );

  ASSERT_EQ( omp::min( 1.5, 12, 5, 21, 7, 9, 0.7f, 1.2 ), 0.7f );

  ASSERT_NE( omp::min( 1.5, 12, 5, 21, 'a', 9, 0.6f, 1.2 ), 5 );
}

TEST( omp_min, user_types )
{
  test_element a( 1 ), b( 2 ), c( 3 ), d( -4 ), t( -1.0 );

  ASSERT_EQ( omp::min( a, 55, c, 34, -2, t, 43 ), -2 );

  ASSERT_EQ( omp::min( 1, a, 1, 1, 1, 1, 1, 1 ), 1 );

  ASSERT_EQ( omp::min( 1, 12, t, 1, 7, 9, 4, 0 ), -1 );

  // TODO: remove warning!
  auto res = omp::min( 1, 'a', -5.0, -3.f, t, 1, 7, 'a', 4, 0 );

  ASSERT_TRUE( typeid( res ).name() == typeid( test_element ).name() );
}

TEST( omp_max, primitive_types )
{
  ASSERT_EQ( omp::max( 2, 55, 6, 34, -2, 54, 43 ), 55.0 );

  ASSERT_EQ( omp::max( 1, 1, 1, 1, 1, 1, 1, 1 ), 1 );

  ASSERT_EQ( omp::max( 1, 12, 5, 1, 7, 9, 4, 0 ), 12 );

  ASSERT_EQ( omp::max( 1, 12, 5, 21, 7, 9, 4, 10 ), 21 );

  ASSERT_EQ( omp::max( 1.5, 12, 5, 21, 21.1, 21.11f, 4, 1.2 ), 21.11f );

  ASSERT_NE( omp::max( 1.5, 12, 5, 21, 7, 9, 0.7f, 1.2 ), 0.7 );

  ASSERT_EQ( omp::max( 1.5, 12, 5, 21, 'a', 9, 0.6f, 1.2 ), 'a' );
}

TEST( omp_max, user_types )
{
  test_element a( 1 ), b( 2 ), c( 3 ), d( -4 ), t( 21.0 );

  ASSERT_EQ( omp::max( a, 55, c, 34, -2, t, 43 ), 55 );

  ASSERT_EQ( omp::max( 1, a, 1, 1, 1, 1, 1, 1 ), a );

  ASSERT_EQ( omp::max( 1, 12, t, 1, 7, 9, 4, 0 ), t );

  // TODO: remove warning!
  auto res = omp::max( 1, 'a', -5.0, -3.f, t, 1, 7, 'a', 4, 0 );

  ASSERT_TRUE( typeid( res ).name() == typeid( test_element ).name() );
}

TEST( omp_sum, primitive_types )
{
  ASSERT_EQ( omp::sum( 2, 55, 6, 34, -2, 54, 43 ), 2 + 55 + 6 + 34 + ( -2 ) + 54 + 43 );

  ASSERT_EQ( omp::sum( 1.5, 12, 5, 21, 7, 9, 4, 1.2 ), 1.5 + 12 + 5 + 21 + 7 + 9 + 4 + 1.2 );

  ASSERT_EQ( omp::sum( 1.5, 12, 5, 21, 7, 9, 0.7f, 1.2 ), 1.5 + 12 + 5 + 21 + 7 + 9 + 0.7f + 1.2 );

  ASSERT_EQ( omp::sum( 1.5, 12, 5, 21, 'a', 9, 0.6f, 1.2 ), 1.5 + 12 + 5 + 21 + 'a' + 9 + 0.6f + 1.2 );
}

TEST( omp_sum, user_types )
{
  test_element a( 1 ), b( 2 ), c( 3 ), d( -4 ), t( -1.0 );

  ASSERT_EQ( omp::sum( a, 55, c, 34, -2, t, 43 ), a + 55 + c + 34 + ( -2 ) + t + 43 );
  
  ASSERT_EQ( omp::sum( 1, 12, t, 1, 7, 9, 4, 0 ), 1 + 12 + ( -1.0 ) + 1 + 7 + 9 + 4 + 0 );

  // TODO: remove warning!
  auto res = omp::sum( 1, 'a', -5.0, -3.f, t, 1, 7, 'a', 4, 0 );

  ASSERT_TRUE( typeid( res ).name() == typeid( test_element ).name() );
}
