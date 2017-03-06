#include <iostream>
#include <vector>
#include <string>
#include <utility>


int main(void) {
    std::string str1 = "hello";
    std::vector<std::string> sv;

    sv.push_back(str1);
    std::cout << "str1: " << str1 << std::endl;

    sv.push_back(std::move(str1));
    std::cout << "str1: " << str1 << std::endl;

    for (auto iter = sv.begin(); iter != sv.end(); ++iter)
        std::cout << *iter << " ";
    std::cout << std::endl;

    return 0;
}
