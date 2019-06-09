#include <iostream>
#include <sstream>
#include <iosfwd>
#include <string>
#include <vector>

template<typename... Args>
std::string StrCat(const Args&... args) {
    std::ostringstream os;
    std::vector<int>{((os << args), 0)...};
    return os.str();
}

int main(void) {
    std::string str = StrCat("1", "2", "3");
    std::cout << str << std::endl;
    return 0;
}
