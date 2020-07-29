#include "omp/utils/common.h"

#include "gtest/gtest.h"

#include <array>

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
  //auto res = omp::min( 1, 'a', -5.0, -3.f, t, 1, 7, 'a', 4, 0 );
  //
  //ASSERT_TRUE( typeid( res ).name() == typeid( test_element ).name() );
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
  //auto res = omp::max( 1, 'a', -5.0, -3.f, t, 1, 7, 'a', 4, 0 );
  //
  //ASSERT_TRUE( typeid( res ).name() == typeid( test_element ).name() );
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
  //auto res = omp::sum( 1, 'a', -5.0, -3.f, t, 1, 7, 'a', 4, 0 );
  //
  //ASSERT_TRUE( typeid( res ).name() == typeid( test_element ).name() );
}

TEST( omp_tuple_map_tuple, one_tuple )
{
  auto initial = std::make_tuple( 1, 2.0, 'a', 4, 5 );
  auto expected = std::make_tuple( 2, 3.0, 'b', 5, 6 );

  auto result = omp::tuple_map( []( const auto& val ) { return val + 1; }, initial );

  ASSERT_EQ( expected, result );
}

TEST( omp_tuple_map_tuple, one_tuple_ref )
{
  auto expected = std::make_tuple( 1, 2.0, 'a', 4, 5 );
  auto result = omp::tuple_map( []( auto& val ) -> auto& { return val; }, expected );

  std::get<0>( result ) -= 1;
  std::get<1>( result ) -= 1;
  std::get<2>( result ) -= 1;
  std::get<3>( result ) -= 1;
  std::get<4>( result ) -= 1;

  ASSERT_EQ( expected, result );
}

TEST( omp_tuple_map_tuple, multiple_tuple )
{
  auto expected1 = std::make_tuple( 1, 2.0, 'a', 4, 5 );
  auto expected2 = std::make_tuple( 5, 4.0, 'z', 2, 1 );

  auto result = omp::tuple_map( []( auto& val1, auto& val2 ) { return val1 + val2; }, expected1, expected2 );

  ASSERT_EQ( std::get<0>( result ), std::get<0>( expected1 ) + std::get<0>( expected2 ) );
  ASSERT_EQ( std::get<1>( result ), std::get<1>( expected1 ) + std::get<1>( expected2 ) );
  ASSERT_EQ( std::get<2>( result ), std::get<2>( expected1 ) + std::get<2>( expected2 ) );
  ASSERT_EQ( std::get<3>( result ), std::get<3>( expected1 ) + std::get<3>( expected2 ) );
  ASSERT_EQ( std::get<4>( result ), std::get<4>( expected1 ) + std::get<4>( expected2 ) );
}

TEST( omp_tuple_map_tuple, multiple_tuple_same )
{
  auto expected1 = std::make_tuple( 1, 2.0, 'a', 4, 5 );

  auto result = omp::tuple_map( []( auto& val1, auto& val2 ) { return val1 + val2; }, expected1, expected1 );

  ASSERT_EQ( std::get<0>( result ), std::get<0>( expected1 ) * 2 );
  ASSERT_EQ( std::get<1>( result ), std::get<1>( expected1 ) * 2 );
  ASSERT_EQ( std::get<2>( result ), std::get<2>( expected1 ) * 2 );
  ASSERT_EQ( std::get<3>( result ), std::get<3>( expected1 ) * 2 );
  ASSERT_EQ( std::get<4>( result ), std::get<4>( expected1 ) * 2 );
}

TEST( omp_tuple_map_tuple, multi_tuple_all_ref )
{
  auto expected1 = std::make_tuple( 1, 2.0, 'a', 4, 5 );
  auto expected2 = std::make_tuple( 5, 4.0, 'z', 2, 1 );

  auto result = omp::tuple_map( []( auto& val1, auto& val2 ) -> auto& { return val1 += val2; },
                                expected1, expected2 );

  ASSERT_EQ( &std::get<0>( result ), &std::get<0>( expected1 ) );
  ASSERT_EQ( &std::get<1>( result ), &std::get<1>( expected1 ) );
  ASSERT_EQ( &std::get<2>( result ), &std::get<2>( expected1 ) );
  ASSERT_EQ( &std::get<3>( result ), &std::get<3>( expected1 ) );
  ASSERT_EQ( &std::get<4>( result ), &std::get<4>( expected1 ) );
}

namespace
{
  struct DummyFunctor
  {
    int& operator()( int& i )
    {
      return i;
    }

    float operator()( float& f )
    {
      return f;
    }

    template<typename T>
    T& operator()( T& d )
    {
      return d;
    }
  };
}

TEST( omp_tuple_map_tuple, multi_tuple_if_not_all_ref_than_value )
{
  auto expected1 = std::make_tuple( 1, 2.0f, 'a', 4, 5 );

  auto result = omp::tuple_map( DummyFunctor(), expected1 );

  ASSERT_NE( &std::get<0>( result ), &std::get<0>( expected1 ) );
  ASSERT_NE( &std::get<1>( result ), &std::get<1>( expected1 ) );
  ASSERT_NE( &std::get<2>( result ), &std::get<2>( expected1 ) );
  ASSERT_NE( &std::get<3>( result ), &std::get<3>( expected1 ) );
  ASSERT_NE( &std::get<4>( result ), &std::get<4>( expected1 ) );
}

TEST( omp_tuple_map_pair, one_pair )
{
  auto expected = std::pair( 1, 2.0 );

  auto result = omp::tuple_map( []( auto& val ) { return val + 5; }, expected );

  ASSERT_EQ( std::get<0>( result ), expected.first + 5 );
  ASSERT_EQ( std::get<1>( result ), expected.second + 5 );
}

TEST( omp_tuple_map_pair, one_pair_ref )
{
    auto expected = std::pair( 1, 2.0 );

    auto result = omp::tuple_map( []( auto& val ) -> auto& { return val; }, expected );

    std::get<0>( result ) = 0;
    std::get<1>( result ) = 0;

    ASSERT_EQ( 0, expected.first  );
    ASSERT_EQ( 0, expected.second );
}

TEST( omp_tuple_map_pair, multiple_pair )
{
    auto expected1 = std::pair( 1, 2.0 );
    auto expected2 = std::pair( 3, 5.0 );

    auto result = omp::tuple_map( []( auto& val1, auto& val2 ) { return val1 + val2; }, expected1, expected2 );

    ASSERT_EQ( std::get<0>( result ), ( expected1.first  + expected2.first  ) );
    ASSERT_EQ( std::get<1>( result ), ( expected1.second + expected2.second ) );
}

TEST( omp_tuple_map_array, one_array )
{
    auto expected = std::array{ 1, 2, 3, 4, 5 };

    auto result = omp::tuple_map( []( const auto& val ) { return val + 1; }, expected );

    ASSERT_EQ( std::get<0>( result ), expected[0] + 1 );
    ASSERT_EQ( std::get<1>( result ), expected[1] + 1 );
    ASSERT_EQ( std::get<2>( result ), expected[2] + 1 );
    ASSERT_EQ( std::get<3>( result ), expected[3] + 1 );
    ASSERT_EQ( std::get<4>( result ), expected[4] + 1 );
}

TEST( omp_tuple_map_array, one_array_ref )
{
    auto expected = std::array{ 1, 2, 3, 4, 5 };

    auto result = omp::tuple_map( []( auto& val ) -> auto& { return val; }, expected );

    ASSERT_EQ( &std::get<0>( result ), &expected[0] );
    ASSERT_EQ( &std::get<1>( result ), &expected[1] );
    ASSERT_EQ( &std::get<2>( result ), &expected[2] );
    ASSERT_EQ( &std::get<3>( result ), &expected[3] );
    ASSERT_EQ( &std::get<4>( result ), &expected[4] );
}

TEST( omp_tuple_map_array, multiple_arrays )
{
    auto expected1 = std::array{ 1, 2, 3, 4, 5 };
    auto expected2 = std::array{ 5, 4, 3, 2, 1 };

    auto result = omp::tuple_map( []( auto& val1, auto& val2 ) -> auto& { return val1 += val2; },
                                  expected1, expected2 );

    std::for_each( std::begin( expected1 ), std::end( expected1 ), []( auto& val ) { val--; } );

    ASSERT_EQ( std::get<0>( result ), expected1[0] );
    ASSERT_EQ( std::get<1>( result ), expected1[1] );
    ASSERT_EQ( std::get<2>( result ), expected1[2] );
    ASSERT_EQ( std::get<3>( result ), expected1[3] );
    ASSERT_EQ( std::get<4>( result ), expected1[4] );
}

TEST( omp_tuple_map_different_types, tuple_pair_array )
{
  auto expected1 = std::make_tuple( 1, 2.0f );
  auto expected2 = std::pair( 1, 2.0f );
  auto expected3 = std::array{ 1, 2 };

  auto result = omp::tuple_map( []( auto& v1, auto& v2, auto& v3 ) { return v1 + v2 + v3; },
                                expected1, expected2, expected3 );

  ASSERT_EQ( std::get<0>( result ), 3 );
  ASSERT_EQ( std::get<1>( result ), 6.f );
}

TEST( omp_tuple_reduce_tuple, one_tuple )
{
  auto expected = std::make_tuple( 1, 2.0, 'a', 4, 5 );
  auto result = omp::tuple_reduce( []( auto accum, auto& val ) { return accum + val; }, 0, expected );

  ASSERT_EQ( result, 1 + 2.0 + 'a' + 4 + 5 );
}

TEST( omp_tuple_reduce_tuple, multiple_tuples )
{
  auto expected1 = std::make_tuple( 1, 2.0, 'a', 4, 5 );
  auto expected2 = std::make_tuple( 5, 4.0, 'z', 2, 1 );

  auto result = omp::tuple_reduce( []( auto accum, auto& val1, auto& val2 ) { return accum + val1 + val2; }, 0,
                                   expected1, std::as_const( expected2 ) );

  ASSERT_EQ( result, 1 + 2.0 + 'a' + 4 + 5 + 5 + 4.0 + 'z' + 2 + 1 );
}

TEST( omp_tuple_reduce_pair, one_pair )
{
  auto expected = std::pair( 2, 3 );

  auto result = omp::tuple_reduce( []( auto accum, auto& val ) { return accum * val; }, 1, expected );

  ASSERT_EQ( result, 6 );
}

TEST( omp_tuple_reduce_pair, multiple_pairs )
{
  auto expected1 = std::pair( 2, 3 );
  auto expected2 = std::pair( 5, 7 );

  auto result = omp::tuple_reduce( []( auto accum, auto& val1, auto& val2 ) { return accum * val1 * val2; }, 1,
                                   std::as_const( expected1 ), expected2 );

  ASSERT_EQ( result, 210 );
}

TEST(omp_tuple_reduce_array, one_array)
{
  auto expected = std::array{ 1, 2, 3, 4, 5 };

  auto result = omp::tuple_reduce( []( auto accum, auto& val ) { return accum * val; }, 1, expected );

  ASSERT_EQ( result, 120 );
}

TEST( omp_tuple_reduce_array, multiple_arrays )
{
  auto expected1 = std::array{ 1, 2, 3, 4, 5 };
  auto expected2 = std::array{ 5, 4, 3, 2, 1 };

  auto result = omp::tuple_reduce( []( auto accum, auto& val1, auto& val2 ) { return accum * val1 * val2; }, 1,
                                   std::as_const( expected1 ), std::as_const( expected2 ) );

  ASSERT_EQ( result, 120 * 120 );
}

TEST( omp_tuple_reduce_different_types, tuple_array_pair )
{
  auto expected1 = std::array{ 1, 2, };
  auto expected2 = std::pair { 3, 4, };
  auto expected3 = std::tuple{ 5, 6, };

  auto result = omp::tuple_reduce( []( auto accum, auto& val1, auto& val2, auto& val3 )
                                   { return accum * val1 * val2 * val3; }, 1,
                                   std::as_const( expected1 ), std::as_const( expected2 ), expected3 );

  ASSERT_EQ( result, 720  );
}
