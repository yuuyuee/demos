#include <iostream>
#include <exception>    // declared the exception, bad_exception
#include <new>          // declared the bad_alloc
#include <typeinfo>     // declared the bad_cast, bad_typeid
#include <functional>   // declared the bad_function_call
#include <memory>       // declared the bad_weak_ptr in shared_ptr.h

int main(void) {
    std::exception exp;

    std::bad_exception be;
    std::bad_alloc ba;
    std::bad_cast bc;
    std::bad_typeid bt;
    std::bad_function_call bfc;
    std::bad_weak_ptr bwp;
#if 0
    std::bad_logic_error ble;
    std::bad_runtime_error bre;
#endif

    std::cout << "exception.what(): " << exp.what() << std::endl;

    std::cout << "bad_exception.what(): " << be.what() << std::endl;
    std::cout << "bad_alloc.what(): " << ba.what() << std::endl;
    std::cout << "bad_cast.what(): " << bc.what() << std::endl;
    std::cout << "bad_typeid.what(): " << bt.what() << std::endl;
    std::cout << "bad_function_call.what(): " << bfc.what() << std::endl;
    std::cout << "bad_weak_ptr.what(): " << bwp.what() << std::endl;
#if 0
    std::cout << "bad_logic_error.what(): " << ble.what() << std::endl;
    std::cout << "bad_runtime_error.what(): " << bre.what() << std::endl;
#endif

    return 0;
}
