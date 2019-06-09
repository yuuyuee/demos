#include <iostream>
#include <algorithm>
#include <list>
#include <forward_list>
#include <deque>

#define TO_STRING(name) (#name)
//#define TO_STRING(name) _TO_STRING(name)

#define PRINT_LIST(lst) do {\
    std::cout << TO_STRING(lst) << ": "; \
    std::for_each(lst.cbegin(), lst.cend(), [](int i) { std::cout << i << " ";}); \
    std::cout << std::endl; \
} while (0)

int main(void) {
    std::list<int> il0({0, 1, 2, 3, 4});
    PRINT_LIST(il0);
    std::list<int> il1({5, 6, 7, 8, 9});
    PRINT_LIST(il1);
    std::list<int> il2(il0);
    il2.splice(il2.end(), il1);
    PRINT_LIST(il2);
    std::cout << "Remove if equal to 2." << std::endl;
    il2.remove_if([](int i) { return i == 2; });
    PRINT_LIST(il2);
    il2.push_back(9);
    PRINT_LIST(il2);
    il2.unique([](int a, int b) { return a == b; });
    PRINT_LIST(il2);
    std::list<int> il3({0, 4, 5, 2});
    il3.merge(il2);
    PRINT_LIST(il3);
    il3.sort([](int a, int b) { return a > b; });
    PRINT_LIST(il3);
    il3.reverse();
    PRINT_LIST(il3);
    std::deque dq;
    return 0;
}

