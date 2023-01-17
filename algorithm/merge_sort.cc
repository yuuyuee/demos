// merge_sort.cc

#include <iostream>
#include <functional>

template <typename Tp, size_t N>
void Print(const char* prefix, const Tp (&a)[N]) {
  std::cout << prefix << " ";
  for (size_t i = 0; i < N; ++i)
    std::cout << a[i] << " ";
  std::cout << std::endl;
}

template <typename Tp, typename Compare>
void Merge(Tp* a, size_t b, size_t m, size_t e, Compare compare) {
  size_t n = e - b;
  Tp* tmp = new Tp[n];
  Tp* ptr = tmp;
  size_t mid = m;
  while (b < m && mid < e) {
    if (compare(a[b], a[mid])) {
      *ptr++ = a[b++];
    } else {
      *ptr++ = a[mid++];
    }
  }
  while (b < m)
    *ptr++ = a[b++];
  while (mid < e)
    *ptr++ = a[mid++];
  for (size_t i = 0; i < n; ++i)
    *a++ = tmp[i];
  delete[] tmp;
}

template <typename Tp, typename Compare>
void MergeSort(Tp* a, size_t n, Compare compare) {
  if (n < 2) return;
  size_t mid = n / 2;
  MergeSort(a, mid, compare);
  MergeSort(a + mid, n - mid, compare);
  Merge(a, 0, mid, n, compare);
}

int main() {
  int a[] = {1, 8, 4, 9, 0, 2, 4};
  Print("B ", a);
  MergeSort(a, 7, std::greater<int>());
  Print("A ", a);


  int a1[] = {1, 8, 4, 9, 0, 2, 4};
  Print("B ", a1);
  MergeSort(a1, 7, std::less_equal<int>());
  Print("A ", a1);
  return 0;
}
