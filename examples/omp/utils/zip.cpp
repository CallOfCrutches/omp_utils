#include "omp/utils/zip.h"
#include <vector>
#include <list>
#include <iostream>
#include <iterator>

struct Test
{

};


void zip_examples()
{
  std::tuple<int, float, int> f;

  std::cout << std::tuple_size_v<decltype(f)>;
  //std::vector<int> a, b, c;
  //
  //auto res = omp::zip( a, std::list<int>(), std::move( b ), c );
  //int i = 1;

}
