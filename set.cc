#include <iostream>
#include <set>

int main(void) {
    std::set<int> s;
    std::multiset<int> ms;

    for (int i = 10; i != 0; i--) {
        s.insert(i);
        s.insert(i);
        ms.insert(i);
        ms.insert(i);
    }
    s.erase(s.begin());
    ms.erase(ms.begin());
    for (auto &iter: s)
        std::cout << iter << " ";
    std::cout << std::endl;
    for (auto &iter: ms)
        std::cout << iter << " ";
    std::cout << std::endl;
    return 0;
}
