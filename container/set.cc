#include <iostream>
#include <set>
<<<<<<< HEAD
#include <map>
#include <unordered_map>
=======
>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772

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

<<<<<<< HEAD
    std::map<int, int> m;
    std::unordered_map<int, int> um;
=======

>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
    return 0;
}
