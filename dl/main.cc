#include <iostream>
#include <cassert>
#include <stdexcept>
#include <string>

#include <dlfcn.h>
#include <unistd.h>

// void PrintEnv() {
//   for (int i = 0; environ[i]; ++i) {
//     std::cout << "environ[" << i << "] = " << environ[i] << std::endl;
//   }
// }

const char* safe_str(const char* p) {
  static const char* empty_str = "";
  return p ? p : empty_str;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " library_path function_name" << std::endl;
    return 0;
  }

  std::string lib(argv[1]);
  std::string fn(argv[2]);

  std::cout << environ[0] << std::endl;
  const char* ld_library_path = safe_str(getenv("LD_LIBRARY_PATH"));
  std::cout << "$LD_LIBRARY_PATH = " << ld_library_path << std::endl;
  std::string lib_dir = lib.substr(0, lib.find_last_of('/'));
  if (!lib_dir.empty()) {
    int ret = setenv("LD_LIBRARY_PATH", lib_dir.c_str(), 1);
    assert(ret == 0);
    std::cout << "setenv(LD_LIBRARY_PATH, " << lib_dir << ")" << std::endl;
  }
  ld_library_path = safe_str(getenv("LD_LIBRARY_PATH"));
  std::cout << "$LD_LIBRARY_PATH = " << ld_library_path << std::endl;

  // PrintEnv();
  void* handle = dlopen(lib.c_str(), RTLD_NOW);
  if (handle == nullptr)
    throw std::runtime_error(dlerror());
  void* symbol = dlsym(handle, fn.c_str());
  if (symbol == nullptr)
    throw std::runtime_error(dlerror());
  void (*fn_a)() = reinterpret_cast<void (*)()>(symbol);
  fn_a();
  dlclose(handle);
  return 0;
}
