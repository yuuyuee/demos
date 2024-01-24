#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unordered_map>
#include <map>
#include <iostream>

#define COUT(...) \
  std::cout << # __VA_ARGS__ << " = " << (__VA_ARGS__) << std::endl

namespace peppa {
inline namespace lts_2023 /*__attribute__((__abi_tag__))*/ {
struct Object {

	Object() {}
	~Object() {}

	void Print() const { COUT(__has_builtin(abi_tag)); }
};

void PrintObject(int a) { COUT(a); }

struct A {};
}
}

peppa::A PrintA() {return peppa::A{}; }

int main() {
    COUT(__cplusplus);
    int a = 100;
    if (__builtin_expect((a > 0), 1))
	    COUT(a);
    peppa::Object obj;
    obj.Print();
    return 0;
}
