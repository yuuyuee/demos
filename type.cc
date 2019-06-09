#include <iostream>
#include <typeinfo>
#include <string>
#include <type_traits>

struct Type {};

template<typename T>
void print_type(const T& v, const std::string& title) {
    (void)v;
    std::cout << title
        << " -> is_pointer: " << std::is_pointer<T>::value
        << ", is_const: " << std::is_const<T>::value
        << ", is_reference: " << std::is_reference<T>::value
        << ", typeid(v).name(): "
        << typeid(v).name() << std::endl;
}

int main(void) {
    int v = 0;
    const std::string s("");
    print_type(v, "v");
    print_type(int(0), "int(0)");
    print_type((const int)(0), "(const int)(0)");
    print_type((const int*)(0), "(const int*)(0)");
    print_type(std::string(), "std::string()");
    print_type(s, "const std::string s("")");
    print_type(Type(), "Type()");

    return 0;
}
