#include "omp/utils/zip.h"
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

struct Test {};

void zip_examples() {
  std::tuple<int, float, int> f;

  std::cout << std::tuple_size_v<decltype(f)>;
  // std::vector<int> a, b, c;
  //
  // auto res = omp::zip( a, std::list<int>(), std::move( b ), c );
  // int i = 1;
}
