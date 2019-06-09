#include <iostream>
#include <string>
#include <utility>

void Print(std::string&& s) {
    std::cout << s << std::endl;
}

void CheckEmpty(const std::string& s) {
    if (s.empty()) {
        std::cout << "String empty" << std::endl;
    } else {
        std::cout << "String not empty: " << s << std::endl;
    }
}

int main(void) {
    std::string str("hello world");
    CheckEmpty(str);
    std::cout << "Print before" << std::endl;
    Print(std::move(str));
    std::cout << "Print after" << std::endl;
    CheckEmpty(str);
    return 0;
}
