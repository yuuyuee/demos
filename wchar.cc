#include <iostream>
#include <exception>
#include <functional>
#include <string>
#include <type_traits>
#include <vector>
#include <sstream>

template<typename T>
struct is_char_pointer: public std::false_type {};

template<>
struct is_char_pointer<char*>: public std::true_type {};

template<>
struct is_char_pointer<const char*>: public std::true_type {};

template<typename T, typename =
    typename std::enable_if<is_char_pointer<T>::value, T>::type>
void Print(T p) {
    std::cout << p << std::endl;
}

template<typename Tp>
struct test {
    template<typename = typename std::enable_if<is_char_pointer<Tp>::value, Tp>::type>
    test(Tp p): p_(p) {}
    ~test() { std::cout << p_ << std::endl; }

    Tp p_;
};

template <typename T>
struct is_char: public std::false_type {};

template <>
struct is_char<char>: public std::true_type {};

int main(void) {
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    std::cout << "experimental" << std::endl;
#endif
    std::cout << __cplusplus << std::endl;
#ifdef __EXCEPTIONS
    std::cout << __EXCEPTIONS << std::endl;
#endif
    //throw(std::exception());
    std::hash<std::string> hash;
    std::cout << hash("123") << std::endl;
    char p[] = {'1', '2', '\0'};
    const char* p1 = "123";
    Print(p);
    Print(p1);
    test<char*> t0(p);
    test<const char*> t1(p1);
    int value __attribute__((unused)) = 0;
    //decltype(int(value), int(value)) index = 60000;
    //std::cout << index << std::endl;
    std::cout << "char: " << is_char<char>::value << std::endl;
    std::cout << "const char: " << is_char<const char>::value << std::endl;
    std::cout << "unsigned char: " << is_char<unsigned char>::value << std::endl;
    std::cout << "const unsigned char: " << is_char<const unsigned char>::value << std::endl;
    std::cout << "is_convertible<int, char>::value: " << std::is_convertible<int, char>::value << std::endl;
    std::cout << "is_same<int*, std::vector<int>::iterator>::value: " << std::is_same<int*, std::vector<int>::iterator>::value << std::endl;
    std::cout << "nchar: " << sizeof('a') << std::endl;
    std::cout << "uchar: " << sizeof(u'a') << std::endl;
    std::cout << "Uchar: " << sizeof(U'a') << std::endl;
    std::cout << "wchar: " << sizeof(L'a') << std::endl;
    std::cout << "wchar_t(a) == a: " << sizeof(wchar_t('a')) << std::endl;
    const wchar_t* cws = L"hello world wchar_t";
    std::wstring ws(cws);
    std::wcout << ws << std::endl;
    auto pp = p1;
    const auto pp1 = p1;
    auto const pp2 = p1;
    std::cout << typeid(pp).name() << std::endl;
    std::cout << typeid(pp1).name() << std::endl;
    std::cout << typeid(pp2).name() << std::endl;
    wchar_t wc('x');
    std::wcout << wc << std::endl;
    std::ostringstream os;
    os << "hello world os" << std::endl;
    std::cout << os.str() << std::endl;
    return 0;
}
