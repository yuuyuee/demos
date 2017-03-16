#include <iostream>
//#include <type_traits>
namespace type_traits {
template<typename T, T v>
struct integral_constant {
    static const T value = v;
    typedef T value_type;
    typedef integral_constant<T, v> type;
    operator value_type() const {
        return value;
    }
};

template<typename T, T v>
const T integral_constant<T, v>::value;

typedef integral_constant<bool, true> true_value;
typedef integral_constant<bool, false> false_value;

// Meta programmming helper types
template<bool Cond, typename Iftrue, typename Iffalse>
struct Conditional { typedef Iftrue type; };

template<typename Iftrue, typename Iffalse>
struct Conditional<false, Iftrue, Iffalse> { typedef Iffalse type; };

template<typename Tp>
struct Not: public integral_constant<bool, !Tp::value> {};

template<typename Tp>
struct remove_const { typedef Tp type; };

template<typename Tp>
struct remove_const<const Tp> { typedef Tp type; };

template<typename Tp>
struct remove_volatile { typedef Tp type; };

template<typename Tp>
struct remove_volatile<volatile Tp> { typedef Tp type; };

template<typename Tp>
struct remove_cv {
    typedef typename remove_const<typename remove_volatile<Tp>::type>::type type;
};

template<typename Tp>
struct remove_reference { typedef Tp type; };

template<typename Tp>
struct remove_reference<Tp&> { typedef Tp type; };

// is_void
template<typename Tp> struct is_void_helper: public false_value {};
template<> struct is_void_helper<void>: public true_value {};
template<typename Tp> struct is_void: public is_void_helper<typename remove_cv<Tp>::type> {};

// is_same
template<typename Tp0, typename Tp1> struct is_same_helper: public false_value {};
template<typename Tp> struct is_same_helper<Tp, Tp>: public true_value {};
template<typename Tp0, typename Tp1> struct is_same: public is_same_helper<typename remove_cv<Tp0>::type, typename remove_cv<Tp1>::type> {};

}

int main(void) {
    std::cout << type_traits::is_same<int, int>::value << std::endl;
    std::cout << type_traits::is_same<int, double>::value << std::endl;
    return 0;
};
