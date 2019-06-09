#include <iostream>
#include <set>
#include <map>
#include <unordered_map>

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

    std::map<int, int> m;
    std::unordered_map<int, int> um;
    return 0;
}
