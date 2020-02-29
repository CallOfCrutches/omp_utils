#include "omp/utils/reversed.h"

#include <iostream>
#include <set>


namespace
{
  void reversed_example()
  {
    std::set elements = { 1,2,3,4,5 };

    for( auto element : omp::reversed( elements ) )
      std::cout << element << " ";

    std::cout << std::endl;
  }
}

void reversed_examples()
{
  std::cout << "reversed examples\n";

  reversed_example();

  std::cout << std::endl;
}
