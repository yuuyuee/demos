#include <iostream>
#include <new>
#include <typeinfo>

//template
template<typename T>
class myclass0 {
public:
    myclass0() {std::cout << "template constructor myclass0" << std::endl;}
    ~myclass0() {std::cout << "template destructor myclass0" << std::endl;}

    template<typename Tp1, bool = __is_empty(Tp1)>
        struct __s_test {
            static void _S_do_it() {
                std::cout << "bool = " << __is_empty(Tp1)<< std::endl;
            }
        };

    template<typename Tp1>
        struct __s_test<Tp1, true> {
            static void _S_do_it() {
                std::cout << "bool = true"<< std::endl;
            }
        };
#if 0
    template<typename Tp1>
        struct __s_test<Tp1, false> {
            static void _S_do_it() {
                std::cout << "bool = false"<< std::endl;
            }
        };
#endif
};


int main(void) {
    myclass0<int> t;

    myclass0<int>::__s_test<int, 1>::_S_do_it();
    myclass0<int>::__s_test<void, 0>::_S_do_it();

    return 0;
}
