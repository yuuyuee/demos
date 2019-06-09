#include <iostream>
<<<<<<< HEAD
#include <string>
#include <system_error>
#include <experimental/filesystem>

int main() {
  throw std::experimental::filesystem::filesystem_error("check failed.", "path1", "path2", std::error_code(EINVAL, std::system_category()));
  return 0;
=======
#include <system_error>

// class error_category
//      const error_category& generic_category() noexcept;
//      const error_category& system_category() noexcept;
//
// enum class errc {};
//
// class error_code
//      error_code make_error_code(errc e)
// template<typename T>
// struct is_error_code_enum;
// template<typename T>
// inline constexpr bool is_error_code_enum_v = is_error_code_enum<T>::value
//
// class error_condition
//      error_condition make_error_condition(errc e)
// template<typename T>
// struct is_error_condition_enum;
// template<typname T>
// inline constexpr bool is_error_condition_enum_v = is_error_condition_enum<T>::value
//
// class system_error

int main() {

    return 0;
>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
}
