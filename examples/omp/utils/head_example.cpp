#include "omp/utils/head.h"

#include <iostream>
#include <set>
#include <vector>


void head_examples()
{
  std::cout << "head examples\n";

  std::vector values = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  for( auto& value : omp::head( values, 5 ) )
  {
    std::cout << value << " ";
  }

  std::cout << std::endl;
}