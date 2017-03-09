#include <iostream>

//template
template<bool B, class T = void>
struct enable_if_c { typedef T type; };

//specification
template<class T>
struct enable_if_c<false, T> {};

template<class Cond, class T = void>
struct enable_if: public enable_if_c<Cond::value, T> {};

//tempalte
template<bool B, class T = void>
struct disable_if_c { typedef T type; };

//specification
template<class T>
struct disable_if_c<true, T> {};

template<class Cond, class T = void>
struct disable_if: public disable_if_c<Cond::value, T> {};

int main(void) {
    return 0;
}
