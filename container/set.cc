#include <iostream>
#include <set>

int main(void) {
    std::set<std::string> s;

    s.insert("yy");
    s.insert("lt");
    s.insert("yy");

    for (auto &e: s)
        std::cout << e << std::endl;
    std::cout << s.size() << std::endl;
    s.erase(s.begin());
    for (auto &e: s)
        std::cout << e << std::endl;
    std::cout << s.size() << std::endl;


    return 0;
}
