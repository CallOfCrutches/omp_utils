#include "omp/utils/zip.h"
#include "gtest/gtest.h"

#include <vector>

TEST( omp_zip, vector_same_type )
{
  std::vector initial1 = { 1, 2, 3, 4, 5 };
  std::vector initial2 = { 6, 7, 8, 9, 0 };

  auto result = omp::zip( std::as_const( initial1 ), std::move( initial2 ) );

  ASSERT_EQ( initial2.size(), 0 );

  auto b1 = std::begin( initial1 ), b2 = std::begin( initial2 );

  auto tup = std::tie( b1, b2 );

  auto res = omp::zip_iterator( tup );

  static_assert(std::is_same_v<decltype( std::get<0>( res.iterators ) ), decltype( std::begin( initial1 ) )>, "LOL");
}
