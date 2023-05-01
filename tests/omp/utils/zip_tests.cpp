#include "omp/utils/zip.h"
#include "gtest/gtest.h"

#include <set>
#include <string_view>
#include <vector>

using namespace std::literals::string_view_literals;

TEST(omp_zip, ref_types) {
  std::vector initial1 = {1, 2, 3, 4, 5};
  std::vector initial2 = {6, 7, 8, 9, 0};

  auto result = omp::zip(std::as_const(initial1), initial2);

  auto f_is_const_ref = std::is_same_v<
      std::add_lvalue_reference_t<std::add_const_t<decltype(initial1)>>,
      std::tuple_element_t<0, std::decay_t<decltype(result.containers())>>>;

  auto s_is_ref = std::is_same_v<
      std::add_lvalue_reference_t<decltype(initial2)>,
      std::tuple_element_t<1, std::decay_t<decltype(result.containers())>>>;

  ASSERT_TRUE(f_is_const_ref && s_is_ref);
}

TEST(omp_zip, val_types) {
  const std::vector initial1 = {1, 2, 3, 4, 5};
  std::vector initial2 = {6, 7, 8, 9, 0};

  auto result = omp::zip(std::move(initial1), std::move(initial2),
                         std::vector<int>{1, 2, 3});

  auto f_is_const_val = std::is_same_v<
      std::add_const_t<decltype(initial1)>,
      std::tuple_element_t<0, std::decay_t<decltype(result.containers())>>>;

  auto s_is_val = std::is_same_v<
      decltype(initial2),
      std::tuple_element_t<1, std::decay_t<decltype(result.containers())>>>;

  auto t_is_val = std::is_same_v<
      std::vector<int>,
      std::tuple_element_t<2, std::decay_t<decltype(result.containers())>>>;

  ASSERT_TRUE(f_is_const_val && s_is_val && t_is_val);
}

TEST(omp_zip, iterator_types) {
  const std::vector initial1 = {1, 2, 3, 4, 5};
  std::vector initial2 = {6, 7, 8, 9, 0};

  auto result = omp::zip_iterator(std::begin(initial1), std::begin(initial2));

  auto f_is_const_it = std::is_same_v<
      decltype(initial1)::const_iterator,
      std::tuple_element_t<0, std::decay_t<decltype(result.iterators())>>>;

  auto s_is_it = std::is_same_v<
      decltype(initial2)::iterator,
      std::tuple_element_t<1, std::decay_t<decltype(result.iterators())>>>;

  ASSERT_TRUE(f_is_const_it && s_is_it);
}

TEST(omp_zip, begin_end) {
  std::vector initial1 = {1, 2, 3, 4, 5};
  std::vector initial2 = {6, 7, 8, 9, 0};

  auto zipped = omp::zip(std::as_const(initial1), initial2);

  {
    auto zip_begin = std::begin(zipped);

    auto f_beg_is_const_it = std::is_same_v<
        decltype(initial1)::const_iterator,
        std::tuple_element_t<0, std::decay_t<decltype(zip_begin.iterators())>>>;

    auto s_beg_is_it = std::is_same_v<
        decltype(initial2)::iterator,
        std::tuple_element_t<1, std::decay_t<decltype(zip_begin.iterators())>>>;
    ;

    ASSERT_TRUE(f_beg_is_const_it && s_beg_is_it);
  }

  {
    auto zip_cbegin = std::begin(std::as_const(zipped));

    auto f_cbeg_is_const_it =
        std::is_same_v<decltype(initial1)::const_iterator,
                       std::tuple_element_t<
                           0, std::decay_t<decltype(zip_cbegin.iterators())>>>;

    auto s_cbeg_is_it =
        std::is_same_v<decltype(initial2)::const_iterator,
                       std::tuple_element_t<
                           1, std::decay_t<decltype(zip_cbegin.iterators())>>>;

    ASSERT_TRUE(f_cbeg_is_const_it && s_cbeg_is_it);
  }
}

TEST(omp_zip, for_loop) {
  std::vector initial1 = {1, 2, 3, 4, 5};
  std::vector initial2 = {6, 7, 8, 9, 0};

  auto zipped = omp::zip(initial1, initial2);

  {
    auto zip_begin = std::begin(zipped), zip_end = std::end(zipped);

    auto it1 = std::cbegin(initial1);
    auto it2 = std::begin(initial2);

    for (; zip_begin != zip_end; ++zip_begin, ++it1, ++it2) {
      auto &[f, s] = *zip_begin;

      ASSERT_EQ(std::get<0>(*zip_begin), *it1);
      ASSERT_EQ(std::get<1>(*zip_begin), *it2);
    }
  }

  {
    auto it1 = std::cbegin(initial1);
    auto it2 = std::begin(initial2);

    for (const auto &[f, s] : zipped) {
      ASSERT_EQ(f, *(it1++));
      ASSERT_EQ(s, *(it2++));
    }
  }
}

TEST(omp_zip, diff_sizes) {
  std::vector initial1 = {1, 2, 3, 4, 5};
  std::vector initial2 = {6, 7, 8, 9, 0, 1, 2, 3, 4, 5};

  auto zipped = omp::zip(initial1, initial2);

  size_t counter = 0;

  for (const auto &_ : zipped)
    ++counter;

  ASSERT_EQ(counter, std::size(initial1));
}

TEST(omp_zip, diff_types) {
  std::vector initial1 = {1, 2, 3, 4, 5};
  std::set initial2 = {"6"sv, "7"sv, "8"sv, "9"sv, "0"sv};

  auto zipped = omp::zip(initial1, initial2);

  auto it1 = std::begin(initial1);
  auto it2 = std::begin(initial2);

  for (const auto &[f, s] : zipped) {
    ASSERT_EQ(f, *(it1++));
    ASSERT_EQ(s, *(it2++));
  }
}
