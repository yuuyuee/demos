#include <iostream>
#include <string>
#include <exception>

// class exception
//      class logic_error: public exception
//          class domain_error: public logic_error
//          class invalid_argument: public logic_error
//          class length_error: public logic_error
//          class out_of_range: public logic_error
//      class rumtime_error: public exception
//          class range_error: public runtime_error
//          class overflow_error: public runtime_error
//          class underflow_error: public runtime_error

template<typename Tp>
void What(const std::string& title, const Tp& e) {
    std::cout << title << ": " << e.what() << std::endl;
}

int main(void) {
    What("std::exception", std::exception());

    What("\tstd::logic_error", std::logic_error("std::logic_error"));
    What("\t\tstd::domain_error", std::domain_error("std::domain_error"));
    What("\t\tstd::invalid_error", std::invalid_argument("std::invalid_argument"));
    What("\t\tstd::length_error", std::length_error("std::invalid_argument"));
    What("\t\tstd::out_of_range", std::out_of_range("std::out_of_range"));

    What("\tstd::runtime_error", std::runtime_error("std::runtime_error"));
    What("\t\tstd::range_error", std::range_error("std::range_error"));
    What("\t\tstd::overflow_error", std::overflow_error("std::overflow_error"));
    What("\t\tstd::underflow_error", std::underflow_error("std::underflow_error"));

    int num = std::uncaught_exceptions();
    std::cout << num << std::endl;

    return 0;
}
