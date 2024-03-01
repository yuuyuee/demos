#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include <unordered_map>
#include <map>
#include <iostream>
#include <string>

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

    std::string s;
    COUT(((const void*) s.c_str()));

    int fd = open(s.c_str(), O_RDONLY);
    COUT(errno);
    COUT(strerror(errno));
    errno = 0;

    fd = open(nullptr, O_RDONLY);
    COUT(errno);
    COUT(strerror(errno));
    errno = 0;

    fd = open("", O_RDONLY);
    COUT(errno);
    COUT(strerror(errno));
    errno = 0;

    COUT(strspn("APP_TYPE", "+-1234567890"));

    return 0;
}
