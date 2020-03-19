#include "omp/utils/common.h"


#include <iostream>
#include <set>


namespace
{
  enum class Channels
  {
    Alpha = 0,
    Red = 1,
    Green = 2,
    Blue = 3,
  };

  void min_example()
  {
    std::cout << "min of elements: 1, 5, 2, 0, -5.4f, 2.1, -5.5 is " << omp::min( 1, 5, 2, 0, -5.4f, 2.1, -5.5 ) << "\n";
  }

  void max_example()
  {
    std::cout << "max of elements: 1, 5, 2, 0, -5.4f, 2.1, -5.5 is " << omp::max( 1, 5, 2, 0, -5.4f, 2.1, -5.5 ) << "\n";
  }

  void sum_example()
  {
    std::cout << "sum of elements: 1, 5, 2, 0, -5.4f, 2.1, -5.5 is " << omp::sum( 1, 5, 2, 0, -5.4f, 2.1, -5.5 ) << "\n";
  }
}

void common_examples()
{
  std::cout << "common examples\n";

  min_example();
  max_example();
  sum_example();

  std::cout << std::endl;
}
