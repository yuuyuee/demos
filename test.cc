#include <iostream>
<<<<<<< HEAD
<<<<<<< HEAD
#include <algorithm>

#include <cxxabi.h>
#include <typeinfo>
#include <string>

template<typename Tp, size_t N>
void Dump(const Tp v[N]) {
	for (size_t i = 0; i < N; ++i)
		std::cout << v[i]  << " ";
	std::cout << std::endl;
}

std::string Demangle(const std::type_info& type) {
	int status = 1;
	size_t len = 1024;
	char buffer[len];
	const char* demangle = abi::__cxa_demangle(type.name(), buffer, &len, &status);
	if (demangle != buffer || status != 0)
		return type.name();
	return buffer;
}

int main() {
	int a0[10];
	Dump<int, 10>(a0);
	int b0[10];
	std::fill_n(b0, 10, 100);
	Dump<int, 10>(b0);
#if defined(__GXX_EXPERIMENTAL_CXX0X__)
	std::cout << __GXX_EXPERIMENTAL_CXX0X__ << std::endl;
	std::cout << __cplusplus << std::endl;
#endif
	std::cout << typeid(Demangle).name() << ":" << Demangle(typeid(Demangle)) << std::endl;

#if defined(__EXCEPTIONS)
  std::cout << "__EXCEPTIONS" << std::endl;
#endif

#if defined(__cpp_exceptions)
  std::cout << "__cpp_exceptions" << std::endl;
#endif
  std::cout << int(char()) << std::endl;
  std::cout << ((-1)%10) << std::endl;
=======

int main() {
  std::cout << "hello world" << std::endl;
>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
=======

int main() {
  std::cout << "hello world" << std::endl;
>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
  return 0;
}
