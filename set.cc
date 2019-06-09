#include <iostream>
#include <set>
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
    return 0;
}
