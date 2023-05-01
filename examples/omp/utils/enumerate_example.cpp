#include "omp/utils/enumerate.h"

#include <iostream>
#include <vector>

namespace {
struct Integer {
  int i{};
};

void enumerate_example() {
  std::vector elements = {Integer{0}, Integer{1}, Integer{2}};

  for (auto &[idx, element] : omp::enumerate(elements, 1))
    std::cout << idx << ": " << element.i << " ";

  std::cout << std::endl;
}
} // namespace

void enumerate_examples() {
  std::cout << "enumerate examples\n";

  enumerate_example();

  std::cout << std::endl;
}
