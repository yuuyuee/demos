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
    std::cout << "real type: " << real_st.type()
              << ", permissions: " << real_st.permissions() << std::endl;
    std::cout << "fake type: " << fake_st.type()
              << ", permissions: " << fake_st.permissions() << std::endl;
    CHECK_MSG(fs::exists(real_st), "real exist", "real not exist");
    CHECK_MSG(fs::exists(fake_st), "fake exist", "fake not exist");
    //symlink_status(), type_present(),
    //permissions_present(), status_known()
    //exists(), is_regular_file(), is_symlink(),
    //is_directory(), is_other()
    std::cout << std::endl;

    //path test
#define PRINT_ERROR_IF(errcode) do {        \
    if ((errcode).value()) {                \
        std::cout << (errcode).value()      \
                  << ": "                   \
                  << (errcode).message()    \
                  << std::endl;             \
    }                                       \
} while (0)

    sys::error_code ec;
    fs::path cur = fs::current_path();
    fs::path src = cur.string() + "/test";
    //fs::path src = "../../cc/boost/test.txt";
    std::cout << "source:        " << src << std::endl;
    std::cout << "root_path:     " << src.root_path() << std::endl;
    std::cout << "relative_path: " << src.relative_path() << std::endl;
    std::cout << "parent_path:   " << src.parent_path() << std::endl;
    std::cout << "filename:      " << src.filename() << std::endl;
    std::cout << "stem:          " << src.stem() << std::endl;
    std::cout << "extension:     " << src.extension() << std::endl;
#define CHECK_BOOL(cond) CHECK_MSG((cond), #cond " - True", #cond " - False")
#if 0
    CHECK_BOOL(src.empty());
//    CHECK_BOOL(src.filename_is_dot());
//    CHECK_BOOL(src.filename_is_dot_dot());
    CHECK_BOOL(src.has_root_path());
    CHECK_BOOL(src.has_root_name());
    CHECK_BOOL(src.has_root_directory());
    CHECK_BOOL(src.has_relative_path());
    CHECK_BOOL(src.has_parent_path());
    CHECK_BOOL(src.has_filename());
    CHECK_BOOL(src.has_stem());
    CHECK_BOOL(src.has_extension());
    CHECK_BOOL(src.is_relative());
    CHECK_BOOL(src.is_absolute());
#endif
#define CLEAR_DO_CHECK(ck, doing) do {  \
    (ck).clear();                       \
    doing;                              \
    PRINT_ERROR_IF(ck);                 \
} while (0)
    //status(), symlink_status(), exists(),
    //is_directory(), is_regular_file(),
    //is_other(), is_symlink(), is_empty()
    fs::path dst = src.parent_path().string()
                 + "/"
                 + src.stem().string()
                 + "_copy"
                 + src.extension().string();
    std::cout << "dst: " << dst << std::endl;
    //CLEAR_DO_CHECK(ec, fs::copy(src, dst, ec));
    //CLEAR_DO_CHECK(ec, fs::copy_file(src, dst, fs::copy_option::overwrite_if_exists, ec));
    //copy_symlink, create_directories, create_directory
    //create_hard_link, create_symlink, equivalent
    //file_size, hard_link_count, last_write_time, permissions
    //read_symlink, remove, remove_all, rename, resize_file
    //space, temp_directory_path, unique_path
    std::cout << std::endl;

    //directory_entry and directory_iterator
    ec.clear();
#if __cplusplus < 201103L
    fs::directory_iterator iter(fs::current_path(), ec);
    PRINT_ERROR_IF(ec);
    for (; iter != fs::directory_iterator(); ++iter) {
        std::cout << "\r- "
                  << iter->path().filename().string()
                  << std::endl;
    }
#else
    for (auto &iter : fs::directory_iterator(fs::current_path())) {
        std::cout << "\r- "
                  << iter.path().filename().string()
                  << std::endl;
    }
#endif
    std::cout << std::endl;

    ec.clear();
    fs::recursive_directory_iterator riter(fs::current_path(), ec);
    PRINT_ERROR_IF(ec);
    for (; riter != fs::recursive_directory_iterator(); ++riter) {
        std::cout << "\r- "
                  << riter->path().filename().string()
                  << std::endl;
    }

    return 0;
}
