#include <iostream>
//#include <type_traits>

template<typename Tp, Tp v>
struct integral_constant {
public:
    static constexpr Tp              value = v;
    typedef Tp                       value_type;
    typedef integral_constant<Tp, v> type;
    constexpr operator value_type() const {
        return value;
    }
};

template<typename Tp, Tp v>
constexpr Tp integral_constant<Tp, v>::value;

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;
template<bool v>
using __bool_constant = integral_constant<bool, v>;



int main(void) {

    std::cout << std::is_void<void>::value << std::endl;
    std::cout << std::is_same<void, int>::value << std::endl;
    std::cout << std::is_same<void, void>::value << std::endl;

    return 0;
}
