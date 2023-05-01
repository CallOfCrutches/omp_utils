#include "omp/utils/make_loaded_list.h"

#include <algorithm>
#include <iostream>
#include <vector>

namespace {

struct Integer {
  int i{};
};

struct IIntegerManager {
  virtual bool LoadDataList(std::vector<int> elements) = 0;

  virtual bool Count(long *count) = 0;

  virtual bool GetRecordByIndex(long index, Integer *data) = 0;
  virtual bool GetRecordByCode(long code, Integer *data) = 0;
};

struct ConcreteIntegerManager : public IIntegerManager {

  virtual bool LoadDataList(std::vector<int> elements) override {
    std::transform(std::cbegin(elements), std::cend(elements),
                   std::back_inserter(Elements),
                   [](auto &&element) { return Integer{element}; });

    return true;
  }

  virtual bool Count(long *count) override {
    *count = std::size(Elements);

    return true;
  }

  virtual bool GetRecordByIndex(long index, Integer *data) override {
    *data = Elements[index];

    return true;
  }

  virtual bool GetRecordByCode(long code, Integer *data) override {
    auto it =
        std::find_if(std::cbegin(Elements), std::cend(Elements),
                     [code](auto &&element) { return element.i == code; });

    if (it != std::cend(Elements)) {
      *data = *it;
      return true;
    }

    return false;
  }

private:
  std::vector<Integer> Elements;
};

void make_loaded_list_example() {
  std::shared_ptr<IIntegerManager> mng =
      std::make_shared<ConcreteIntegerManager>();

  auto list = omp::make_loaded_list(mng, std::vector{5, 8, 9, 12, 1, 3, 4, 80});

  auto element = list[omp::by_code{12}];

  std::cout << std::size(list) << " " << list[omp::by_code{12}].i << " "
            << list[2].i << std::endl;
}

void make_loaded_list_for_range_example() {
  std::shared_ptr<IIntegerManager> mng =
      std::make_shared<ConcreteIntegerManager>();

  auto list = omp::make_loaded_list(mng, std::vector{5, 8, 9, 12, 1, 3, 4, 80});

  for (auto &element : list)
    std::cout << element.i << " ";

  std::cout << std::endl;
}
} // namespace

void make_loaded_list_examples() {
  std::cout << "make_loaded_list examples\n";

  make_loaded_list_example();
  make_loaded_list_for_range_example();

  std::cout << std::endl;
}
