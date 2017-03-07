#include <iostream>
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/system/error_code.hpp>
#include <boost/filesystem.hpp>

namespace fs = ::boost::filesystem;
namespace sys = ::boost::system;

int main(void) {
    fs::path current = fs::current_path();
    fs::path initial = fs::current_path();

    std::cout << "current_path: " << current.string() << std::endl;
    std::cout << "initial_path: " << current.string() << std::endl;
    std::cout << std::endl;

    //error_code test
    sys::error_code err;

    std::cout << "error_code initial value: " << err.value() << std::endl;
    std::cout << "error_code category.name: " << err.category().name() << std::endl;
    err = sys::posix_error::make_error_code(sys::posix_error::interrupted);
    std::cout << "error_code last value: " << err.value() << std::endl;
    std::cout << "error_code message: " << err.message() << std::endl;
    std::cout << "error_code category.name: " << err.category().name() << std::endl;
    std::cout << "error_code category.message: " << err.category().message(err.value()) << std::endl;
    std::cout << std::endl;

    //status test;
    fs::path real = current, fake = current;
    real /= "real";
    fake /= "fake";
    fs::file_status real_st = fs::status(real);
    fs::file_status fake_st = fs::status(fake);

#define CHECK_MSG(cond, truemsg, falsemsg)  \
    if ((cond)) {                           \
        std::cout << truemsg << std::endl;  \
    } else {                                \
        std::cout << falsemsg << std::endl; \
    }

    std::cout << "real path: " << real.string() << std::endl;
    std::cout << "fake path: " << fake.string() << std::endl;
    std::cout << "real type: " << real_st.type() << ", permissions: " << real_st.permissions() << std::endl;
    std::cout << "fake type: " << fake_st.type() << ", permissions: " << fake_st.permissions() << std::endl;
    CHECK_MSG(fs::exists(real_st), "real file exist", "real file not exist");
    CHECK_MSG(fs::exists(fake_st), "fake file exist", "fake file not exist");
    //symlink_status(), type_present(), permissions_present(), status_known()
    //exists(), is_regular_file(), is_symlink(), is_directory(), is_other()

    //path test

    return 0;
}
