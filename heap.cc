#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <functional>
#include <iterator>

template<typename Vec>
void PrintVector(const Vec& vec) {
  static int index = 0;
  std::cout << index++ << ": ";
  for (const auto& e: vec)
    std::cout << e << " ";
  std::cout << std::endl;
}


struct IterGreaterValue {
  //typedef std::iterator_traits<RandomAccessIterator>::value_type value_type;
  //using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;
  template<typename RandomAccessIterator>
  bool operator()(RandomAccessIterator iter,
      typename std::iterator_traits<RandomAccessIterator>::value_type& val) {
    return *iter > val;
  }

  template<typename value_type>
  bool operator()(const value_type& left, const value_type& right) {
    return left > right;
  }
};

int main(void) {
  std::vector<int> iv({4, 6, 2, 0, 8, 5});
  PrintVector(iv); // 0

  std::make_heap(iv.begin(), iv.end(), IterGreaterValue());
  PrintVector(iv); // 1

  iv.push_back(9);
  PrintVector(iv); // 2
  std::push_heap(iv.begin(), iv.end(), IterGreaterValue());
  PrintVector(iv); // 3

  std::pop_heap(iv.begin(), iv.end(), IterGreaterValue());
  std::cout << "iv[n - 1] = " << iv[iv.size() - 1] << std::endl;
  std::cout << "iv.size() = " << iv.size() << std::endl;
  PrintVector(iv); // 4

  iv.resize(iv.size() - 1);

  std::pop_heap(iv.begin(), iv.end(), IterGreaterValue());
  std::cout << "iv[n - 1] = " << iv[iv.size() - 1] << std::endl;
  std::cout << "iv.size() = " << iv.size() << std::endl;
  PrintVector(iv); // 5

  std::cout << "is_heap " << std::is_heap(iv.begin(), iv.end(), IterGreaterValue()) << std::endl;

  return 0;
}

