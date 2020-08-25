#include "omp/utils/zip.h"
#include "gtest/gtest.h"

#include <type_traits>
#include <vector>

TEST( omp_zip, ref_types )
{
  std::vector initial1 = { 1, 2, 3, 4, 5 };
  std::vector initial2 = { 6, 7, 8, 9, 0 };

  auto result = omp::zip( std::as_const( initial1 ), initial2 );

  auto f_is_const_ref = std::is_same_v<std::add_lvalue_reference_t<std::add_const_t<decltype( initial1 )>>,
                                       std::tuple_element_t<0, std::decay_t<decltype( result.containers() )>>>;

  auto s_is_ref = std::is_same_v<std::add_lvalue_reference_t<decltype( initial2 )>,
                                 std::tuple_element_t<1, std::decay_t<decltype( result.containers() )>>>;

  ASSERT_TRUE( f_is_const_ref && s_is_ref );
}

TEST( omp_zip, val_types )
{
    const std::vector initial1 = { 1, 2, 3, 4, 5 };
    std::vector initial2 = { 6, 7, 8, 9, 0 };

    auto result = omp::zip( std::move( initial1 ), std::move( initial2 ), std::vector<int>{ 1,2,3 } );

    auto f_is_const_val = std::is_same_v<std::add_const_t<decltype( initial1 )>,
                                         std::tuple_element_t<0, std::decay_t<decltype( result.containers() )>>>;

    auto s_is_val = std::is_same_v<decltype( initial2 ),
                                   std::tuple_element_t<1, std::decay_t<decltype( result.containers() )>>>;

    auto t_is_val = std::is_same_v<std::vector<int>,
                                   std::tuple_element_t<2, std::decay_t<decltype( result.containers() )>>>;

    ASSERT_TRUE( f_is_const_val && s_is_val && t_is_val );
}

TEST( omp_zip, iterator_types )
{
}

TEST( omp_zip, begin_end )
{
}

TEST( omp_zip, for_loop )
{
}

TEST( omp_zip, diff_sizes )
{
}

TEST( omp_zip, diff_types )
{
}
