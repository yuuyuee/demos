#include <iostream>

class DisabledHeapObject {
public:
  DisabledHeapObject() {
    std::cout << "DisabledHeapObject::DisabledHeapObject()" << std::endl;
  }

  ~DisabledHeapObject() {
    std::cout << "DisabledHeapObject::~DisabledHeapObject()" << std::endl;
  }

  void* operator new(std::size_t) = delete;
};

int main() {
  DisabledHeapObject obj; // Success
  //DisabledHeapObject* ptr = new DisabledHeapObject; // failed
  return 0;
}
