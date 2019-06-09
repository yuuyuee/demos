#include <iostream>
#include <limits>

struct test {
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  size_t Delegate() const {
    return MaxSize();
  }
private:
  static constexpr size_type kMaxSize = std::numeric_limits<difference_type>::max();
  static constexpr size_type MaxSize() {
    return kMaxSize;
  }
};


int main() {
  test t;
  std::cout << t.Delegate() << std::endl;
  return 0;
}
