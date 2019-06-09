#include <iostream>
#include <set>
<<<<<<< HEAD
#include <unordered_set>
#include <map>
#include <unordered_map>

int main(void) {
    std::set<int> si;
    std::multiset<int> msi;
    std::unordered_set<int> usi;
    std::unordered_multiset<int> umsi;

    std::map<int, int> mi;
    std::multimap<int, int> mmi;
    std::unordered_map<int, int> umi;
    std::unordered_multimap<int, int> ummi;
=======

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
>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
    return 0;
}
