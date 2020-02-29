#include "omp/utils/range.h"

#include <iostream>
#include <iterator>
#include <utility>


namespace
{
  void simple_range_example()
  {
    for( auto value : omp::range( 10 ) )
      std::cout << value << " ";

    std::cout << std::endl;
  }

  void iterators_range_example()
  {
    auto range_wrapper = omp::range( 10 );

    std::copy( std::cbegin( range_wrapper ), std::cend( range_wrapper ),
               std::ostream_iterator<std::int64_t>( std::cout, " " ) );

    std::cout << std::endl;
  }

  void step_range_example()
  {
    for( auto value : omp::range( 50, 5, -8 ) )
      std::cout << value << " ";

    std::cout << std::endl;
  }
}

void range_examples()
{
  std::cout << "range examples\n";

  simple_range_example();
  iterators_range_example();
  step_range_example();

  std::cout << std::endl;
}
