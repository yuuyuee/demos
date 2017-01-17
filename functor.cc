#include <vector>
#include <algorithm>
#include <iostream>

template <typename T>
class print {
public:
    void operator()(const T &elem) {
        std::cout << elem << ' ';
    }
};

int main(void) {
    int ia[] = {0, 1, 2, 3, 4, 5};
    std::vector<int> iv(ia, ia + 6);

    for_each(iv.begin(), iv.end(), print<int>());
    return 0;
}
