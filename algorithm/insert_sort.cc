// insert_sort.cc

#include <iostream>
#include <functional>

template <typename Tp, size_t N>
void Print(const char* prefix, const Tp (&a)[N]) {
  std::cout << prefix << " ";
  for (size_t i = 0; i < N; ++i)
    std::cout << a[i] << " ";
  std::cout << std::endl;
}

template <typename Tp, size_t N, typename Compare = std::greater<Tp>>
void InsertSort(Tp (&a)[N]) {
  Compare compare;
  for (size_t i = 1; i < N; ++i) {
    for (size_t j = i; j > 0 && compare(a[j], a[j - 1]); --j) {
      Tp val = a[j];
      a[j] = a[j - 1];
      a[j - 1] = val;
    }
  }
}

int main() {
  int a[] = {1, 8, 4, 9, 0, 2, 4};
  Print("B ", a);
  InsertSort(a);
  Print("A ", a);


  int a1[] = {1, 8, 4, 9, 0, 2, 4};
  Print("B ", a1);
  InsertSort<int, 7, std::less<int>>(a1);
  Print("A ", a1);
  return 0;
}
