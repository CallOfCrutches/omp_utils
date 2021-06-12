#include "omp/utils/tuple_map_reduce.h"

#include "gtest/gtest.h"

#include <type_traits>
#include <algorithm>
#include <array>


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

  auto result = omp::tuple_map( []( auto& val1, auto&& val2 ) -> auto& { return val1 += val2; },
                                expected1, std::move( expected2 ) );

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
    char&& operator()( char& c )
    {
      return std::move( c );
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

TEST( omp_tuple_map_tuple, type_depends_on_functor_res )
{
  auto expected1 = std::make_tuple( 1, 2.0f, 'a', 4, 5 );

  auto result = omp::tuple_map( DummyFunctor(), expected1 );

  auto t_frst = std::is_same_v<int&  , std::tuple_element_t<0, decltype( result )>>;
  auto t_scnd = std::is_same_v<float , std::tuple_element_t<1, decltype( result )>>;
  auto t_thrd = std::is_same_v<char&&, std::tuple_element_t<2, decltype( result )>>;
  auto t_frth = std::is_same_v<int&  , std::tuple_element_t<3, decltype( result )>>;
  auto t_ffth = std::is_same_v<int&  , std::tuple_element_t<4, decltype( result )>>;

  ASSERT_TRUE( t_frst && t_scnd && t_thrd && t_frth && t_ffth );
}

TEST( omp_tuple_map_tuple, type_depends_on_lambda_rvalue__ref_res )
{
  auto expected1 = std::make_tuple( 1, 2.0f, 'a', 4, 5 );
  
  auto result = omp::tuple_map( []( auto&& v ) -> decltype( auto ) { return std::forward<decltype( v )>( v ); },
      std::move( expected1 ) );

  auto t_frst = std::is_same_v<int&&  , std::tuple_element_t<0, decltype( result )>>;
  auto t_scnd = std::is_same_v<float&&, std::tuple_element_t<1, decltype( result )>>;
  auto t_thrd = std::is_same_v<char&& , std::tuple_element_t<2, decltype( result )>>;
  auto t_frth = std::is_same_v<int&&  , std::tuple_element_t<3, decltype( result )>>;
  auto t_ffth = std::is_same_v<int&&  , std::tuple_element_t<4, decltype( result )>>;

  ASSERT_TRUE( t_frst && t_scnd && t_thrd && t_frth && t_ffth );
}

TEST( omp_tuple_map_tuple, type_depends_on_lambda_rvalue_res )
{
  auto expected1 = std::make_tuple( 1, 2.0f, 'a', 4, 5 );
  
  auto result = omp::tuple_map( []( auto&& v ) -> auto { return v; }, std::move( expected1 ) );

  auto t_frst = std::is_same_v<int  , std::tuple_element_t<0, decltype( result )>>;
  auto t_scnd = std::is_same_v<float, std::tuple_element_t<1, decltype( result )>>;
  auto t_thrd = std::is_same_v<char , std::tuple_element_t<2, decltype( result )>>;
  auto t_frth = std::is_same_v<int  , std::tuple_element_t<3, decltype( result )>>;
  auto t_ffth = std::is_same_v<int  , std::tuple_element_t<4, decltype( result )>>;

  ASSERT_TRUE( t_frst && t_scnd && t_thrd && t_frth && t_ffth );
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

namespace
{
  struct DummyFunctorAgain
  {
    constexpr int operator()( int a, int b ) { return a * b; }
    constexpr int operator()( int a ) { return a * a; }
  };
}

TEST( constexpr_context, omp_tuple_reduce )
{
  std::array<int, omp::tuple_reduce( DummyFunctorAgain(), 1, std::make_tuple( 1, 2, 3 ) ) > t;
  ASSERT_EQ( t.max_size(), 6 );
}

TEST( constexpr_context, omp_tuple_map )
{
  std::array<int, std::get<2>( omp::tuple_map( DummyFunctorAgain(), std::make_tuple( 1, 2, 3 ) ) ) > t;
  ASSERT_EQ( t.max_size(), 9 );
}
