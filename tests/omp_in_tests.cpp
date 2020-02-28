#include "gtest/gtest.h"
#include "omp/utils/omp_in.h"
#include <vector>
#include <set>
#include <list>
#include <unordered_map>
#include <forward_list>


TEST( omp_in, integer_elements )
{
  const int one = 1, three = 3;

  ASSERT_TRUE( omp::in( one, 4, 5, 3, 1, 2 ) );

  ASSERT_FALSE( omp::in( three, 1, 5, 6, 2, 1, 8 ) );
}

TEST( omp_in, floating_point_elements )
{
  ASSERT_TRUE( omp::in( 1.51, 4, 5, 1.51, 1.1, 2.3, 1.5 ) );

  ASSERT_FALSE( omp::in( 2.1, 4, 2.2, 1, 0.9, 1, 8 ) );
}

TEST( omp_in, string_elements )
{
  const std::string dummy = "test";

  ASSERT_TRUE( omp::in( dummy, "firefox", "safari", "test" ) );
}

namespace
{
  enum class TestEnumClass
  {
    A,
    B,
    C,
    D,
  };

  enum TestEnum
  {
    A,
    B,
    C,
    D,
  };
}

TEST( omp_in, enum_elements )
{
  ASSERT_TRUE( omp::in( TestEnumClass::A, TestEnumClass::B, 
                          TestEnumClass::C, TestEnumClass::A ) );

  ASSERT_TRUE( omp::in( A, B, C, 1, 5, D, 0 ) );
}

namespace
{
  struct TestSimple
  {
    inline static int variable = 42;

    auto begin() const noexcept
    {
      return &variable;
    }

    auto end() const noexcept
    {
      return ( &variable ) + 1;
    }
  };
}

TEST( omp_in, container_in_simple )
{
  ASSERT_TRUE( omp::in( 42, TestSimple() ) );
}

namespace
{
  struct TestWithFind
  {
    using key_type = int;

    inline static int variable = 42;

    auto begin() const noexcept
    {
      return nullptr;
    }

    auto end() const noexcept
    {
      return nullptr;
    }

    auto find( const key_type& ) const noexcept
    {
      return &variable;
    }
  };
}

TEST( omp_in, container_in_with_find )
{
  ASSERT_TRUE( omp::in( 42, TestWithFind() ) );
}

TEST( omp_in, container_in_stls )
{
  {
    const std::set<int> set_elements = { 1, 42, 5 };
    ASSERT_TRUE( omp::in( 42, set_elements ) );
  }

  {
    const std::vector<int> vec_elements = { 1, 42, 5 };
    ASSERT_TRUE( omp::in( 42, vec_elements ) );
  }

  {
    const std::list<int> list_elements = { 1, 2, 6, 1 };
    ASSERT_FALSE( omp::in( 42, list_elements ) );
  }

  {
    const std::forward_list<int> fw_elements = { 1, 2, 6, 1 };
    ASSERT_FALSE( omp::in( 42, fw_elements ) );
  }

  {
    const std::unordered_map<int, double> um_elements = { {1,5.0}, {62, 25.5}, {11, 242} };
    ASSERT_FALSE( omp::in( 42, um_elements ) );
  }
}
