#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "omp_utils/make_loaded_list.h"
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

TEST( utils_make_loaded_list, empty_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 0 ), Return( true ) ) );

  auto list = utils::make_loaded_list( mock );

  ASSERT_TRUE( std::empty( list ) );
  ASSERT_TRUE( std::cbegin( list ) == std::cend( list ) );
}

TEST( utils_make_loaded_list, number_of_elements_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 5 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( Return( true ) );

  auto list = utils::make_loaded_list( mock );

  ASSERT_FALSE( std::empty( list ) );
  ASSERT_TRUE( std::size( list ) == 5 );
  ASSERT_TRUE( std::cbegin( list ) + 5 == std::cend( list ) );
}

TEST( utils_make_loaded_list, get_value_iterator_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 1 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( DoAll( SetArgPointee<1>( TestValue{ 42 } ), Return( true ) ) );

  auto list = utils::make_loaded_list( mock );

  ASSERT_TRUE( *std::cbegin( list ) == TestValue{ 42 } );
}

TEST( utils_make_loaded_list, get_value_operator_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 1 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( DoAll( SetArgPointee<1>( TestValue{ 42 } ), Return( true ) ) );

  auto list = utils::make_loaded_list( mock );

  ASSERT_TRUE( list[0] == TestValue{ 42 } );
}

TEST( utils_make_loaded_list, get_value_at_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 1 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( DoAll( SetArgPointee<1>( TestValue{ 42 } ), Return( true ) ) );

  auto list = utils::make_loaded_list( mock );

  ASSERT_TRUE( list.at( 0 ) == TestValue{ 42 } );
}

TEST( utils_make_loaded_list, get_value_at_exception_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 1 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( DoAll( SetArgPointee<1>( TestValue{ 42 } ), Return( true ) ) );

  auto list = utils::make_loaded_list( mock );

  ASSERT_THROW( list.at( 12 ), std::logic_error );
}

TEST( utils_make_loaded_list, get_value_operator_by_code_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 1 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByCode )
    .WillRepeatedly( []( long code, TestValue* value ) { value->dummy = code; return true; } );

  auto list = utils::make_loaded_list( mock );

  ASSERT_TRUE( list[utils::by_code{ 11 }] == TestValue{ 11 } );
}

TEST( utils_make_loaded_list, many_elements_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 10 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( []( long index, TestValue* value ) { value->dummy = index; return true; } );

  auto list = utils::make_loaded_list( mock );

  const std::vector expecting = { 0,1,2,3,4,5,6,7,8,9 };

  auto lIt = std::cbegin( list );
  auto eIt = std::cbegin( expecting );

  ASSERT_TRUE( std::equal( std::cbegin( list ), std::cend( list ),
                           std::cbegin( expecting ), std::cend( expecting ),
                           []( auto& f, auto& s ) { return f.dummy == s; } ) );
}

/*TEST( utils_make_loaded_list, many_elements_reversed_test )
{
  auto mock = std::make_shared<TestMock>();

  EXPECT_CALL( *mock.get(), LoadDataList );

  EXPECT_CALL( *mock.get(), Count )
    .WillRepeatedly( DoAll( SetArgPointee<0>( 10 ), Return( true ) ) );

  EXPECT_CALL( *mock.get(), GetRecordByIndex )
    .WillRepeatedly( []( long index, TestValue* value ) { value->dummy = index; return true; } );

  auto list = utils::make_loaded_list( mock );

  const std::vector expecting = { 0,1,2,3,4,5,6,7,8,9 };

  ASSERT_TRUE( std::equal( std::crbegin( list ), std::crend( list ),
                           std::crbegin( expecting ), std::crend( expecting ),
                           []( auto& f, auto& s ) { return f.dummy == s; } ) );
}*/

/*TEST( utils_make_loaded_list, empty_test )
{

}

TEST( utils_make_loaded_list, empty_test )
{

}

TEST( utils_make_loaded_list, empty_test )
{

}

TEST( utils_make_loaded_list, empty_test )
{

}

TEST( utils_make_loaded_list, empty_test )
{

}*/
