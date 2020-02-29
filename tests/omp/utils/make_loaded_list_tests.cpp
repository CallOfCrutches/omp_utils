#include "omp/utils/make_loaded_list.h"
#include "omp/utils/reversed.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <memory>


namespace
{
  struct TestValue
  {
    long dummy{};
  };

  bool operator==( const TestValue& f, const TestValue& s ) noexcept
  {
    return f.dummy == s.dummy;
  }

  struct ITestInterface
  {
    virtual ~ITestInterface() = default;

    virtual void LoadDataList() = 0;

    virtual bool Count( long* ) = 0;

    virtual bool GetRecordByCode( long code, TestValue* value ) = 0;
    virtual bool GetRecordByIndex( long idx, TestValue* value ) = 0;
  };

  struct TestMock: public ITestInterface
  {
    MOCK_METHOD( void, LoadDataList, ( ), ( override ) );

    MOCK_METHOD( bool, Count, ( long* ), ( override ) );

    MOCK_METHOD( bool, GetRecordByCode, ( long, TestValue* ), ( override ) );
    MOCK_METHOD( bool, GetRecordByIndex, ( long, TestValue* ), ( override ) );
  };
}

using testing::DoAll;

using testing::Return;
using testing::SetArgPointee;
using testing::_;

TEST( omp_make_loaded_list, empty_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 0 ), Return( true ) ) );

  auto list = omp::make_loaded_list( mock );

  ASSERT_TRUE( std::empty( list ) );
  ASSERT_TRUE( std::cbegin( list ) == std::cend( list ) );
}

TEST( omp_make_loaded_list, number_of_elements_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 5 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( Return( true ) );

  auto list = omp::make_loaded_list( mock );

  ASSERT_FALSE( std::empty( list ) );
  ASSERT_TRUE( std::size( list ) == 5 );
  ASSERT_TRUE( std::cbegin( list ) + 5 == std::cend( list ) );
}

TEST( omp_make_loaded_list, get_value_iterator_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 1 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( DoAll( SetArgPointee<1>( TestValue{ 42 } ), Return( true ) ) );

  auto list = omp::make_loaded_list( mock );

  ASSERT_TRUE( *std::cbegin( list ) == TestValue{ 42 } );
}

TEST( omp_make_loaded_list, get_value_operator_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 1 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( DoAll( SetArgPointee<1>( TestValue{ 42 } ), Return( true ) ) );

  auto list = omp::make_loaded_list( mock );

  ASSERT_TRUE( list[0] == TestValue{ 42 } );
}

TEST( omp_make_loaded_list, get_value_at_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 1 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( DoAll( SetArgPointee<1>( TestValue{ 42 } ), Return( true ) ) );

  auto list = omp::make_loaded_list( mock );

  ASSERT_TRUE( list.at( 0 ) == TestValue{ 42 } );

  ASSERT_THROW( list.at( 12 ), std::logic_error );
}

TEST( omp_make_loaded_list, get_value_operator_by_code_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 1 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByCode )
    .WillRepeatedly( []( long code, TestValue* value ) { value->dummy = code; return true; } );

  auto list = omp::make_loaded_list( mock );

  ASSERT_TRUE( list[omp::by_code{ 11 }] == TestValue{ 11 } );
}

TEST( omp_make_loaded_list, simple_iterator )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 10 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( []( long index, TestValue* value ) { value->dummy = index + 1; return true; } );

  auto list = omp::make_loaded_list( mock );

  ASSERT_EQ( std::cend( list ) - std::cbegin( list ), 10 );

  ASSERT_EQ( std::cbegin( list )->dummy, 1 );

  ASSERT_EQ( ( ++std::cbegin( list ) )->dummy, 2 );

  ASSERT_EQ( ( std::cbegin( list ) + 5 )->dummy, 6 );
  ASSERT_EQ( ( std::cbegin( list ) + 2 )->dummy, 3 );
  ASSERT_EQ( ( std::cbegin( list ) + 4 )->dummy, 5 );

  ASSERT_EQ( ( --std::cend( list ) )->dummy, 10 );
}

TEST( omp_make_loaded_list, simple_iterator_comparisons )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 10 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( []( long index, TestValue* value ) { value->dummy = index + 1; return true; } );

  auto list = omp::make_loaded_list( mock );

  ASSERT_LT( std::cbegin( list )    , std::cend( list )     );
  ASSERT_LE( std::cbegin( list ) + 5, std::cend( list ) - 5 );
  ASSERT_EQ( std::cbegin( list ) + 5, std::cend( list ) - 5 );
  ASSERT_NE( std::cbegin( list ) + 4, std::cend( list ) - 4 );
  ASSERT_GT( std::cbegin( list ) + 7, std::cend( list ) - 7 );
  ASSERT_GE( std::cbegin( list ) + 5, std::cend( list ) - 5 );
}

TEST( omp_make_loaded_list, many_elements_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 10 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( []( long index, TestValue* value ) { value->dummy = index + 1; return true; } );

  auto list = omp::make_loaded_list( mock );

  const std::vector expecting = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  ASSERT_TRUE( std::equal( std::cbegin( list ), std::cend( list ),
                           std::cbegin( expecting ), std::cend( expecting ),
                           []( auto& f, auto& s ) { return f.dummy == s; } ) );
}

TEST( omp_make_loaded_list, reversed_iterator )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 10 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( []( long index, TestValue* value ) { value->dummy = index + 1; return true; } );

  auto list = omp::make_loaded_list( mock );

  ASSERT_EQ( std::crend( list ) - std::crbegin( list ), 10 );

  ASSERT_EQ( std::crbegin( list )->dummy, 10 );

  ASSERT_EQ( ( ++std::crbegin( list ) )->dummy, 9 );

  ASSERT_EQ( ( std::crbegin( list ) + 5 )->dummy, 5 );
  ASSERT_EQ( ( std::crbegin( list ) + 2 )->dummy, 8 );
  ASSERT_EQ( ( std::crbegin( list ) + 4 )->dummy, 6 );

  ASSERT_EQ( ( --std::crend( list ) )->dummy, 1 );
}

TEST( omp_make_loaded_list, reversed_iterator_comparisons )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 10 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( []( long index, TestValue* value ) { value->dummy = index + 1; return true; } );

  auto list = omp::make_loaded_list( mock );

  ASSERT_LT( std::crbegin( list )    , std::crend( list )     );
  ASSERT_LE( std::crbegin( list ) + 5, std::crend( list ) - 5 );
  ASSERT_EQ( std::crbegin( list ) + 5, std::crend( list ) - 5 );
  ASSERT_NE( std::crbegin( list ) + 4, std::crend( list ) - 4 );
  ASSERT_GT( std::crbegin( list ) + 7, std::crend( list ) - 7 );
  ASSERT_GE( std::crbegin( list ) + 5, std::crend( list ) - 5 );
}

TEST( omp_make_loaded_list, many_elements_reversed_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 10 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( []( long index, TestValue* value ) { value->dummy = index + 1; return true; } );

  auto list = omp::make_loaded_list( mock );

  const std::vector expecting = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  ASSERT_TRUE( std::equal( std::crbegin( list ), std::crend( list ),
                           std::crbegin( expecting ), std::crend( expecting ),
                           []( auto& f, auto& s ) { return f.dummy == s; } ) );
}

TEST( omp_make_loaded_list, with_omp_reversed )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 10 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( []( long index, TestValue* value ) { value->dummy = index + 1; return true; } );

  auto reversed = omp::reversed( omp::make_loaded_list( mock ) );

  std::vector expecting = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

  ASSERT_TRUE( std::equal( std::cbegin( reversed ), std::cend( reversed ),
                           std::cbegin( expecting ), std::cend( expecting ),
                           []( auto&& f, auto&& s ) { return f.dummy == s; } ) );
}
