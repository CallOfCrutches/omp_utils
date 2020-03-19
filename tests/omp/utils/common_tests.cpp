#include "omp/utils/common.h"

#include "gtest/gtest.h"

TEST( omp_min, integer_elements )
{
  omp::min( 2, 55, 6, 34, -2, 54, 43.5 );

  //const int one = 1, three = 3;
  //
  //ASSERT_TRUE( omp::in( one, 4, 5, 3, 1, 2 ) );
  //
  //ASSERT_FALSE( omp::in( three, 1, 5, 6, 2, 1, 8 ) );
}
