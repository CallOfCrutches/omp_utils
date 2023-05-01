#include "omp/utils/in.h"

#include <iostream>
#include <set>

namespace {
enum class Channels {
  Alpha = 0,
  Red = 1,
  Green = 2,
  Blue = 3,
};

void simple_omp_in_example() {
  auto channel = Channels::Red;

  if (omp::in(channel, Channels::Red, Channels::Green, Channels::Blue))
    std::cout << "Color channel\n";
  else
    std::cout << "Alpha channel\n";
}

void container_omp_in_example() {
  const auto channel = Channels::Alpha;

  const std::set channels = {Channels::Red, Channels::Green, Channels::Blue};

  if (omp::in(channel, channels))
    std::cout << "Color channel\n";
  else
    std::cout << "Alpha channel\n";
}
} // namespace

void in_examples() {
  std::cout << "in examples\n";

  simple_omp_in_example();
  container_omp_in_example();

  std::cout << std::endl;
}
