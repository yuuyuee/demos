#include <iostream>
#include <vector>

int main(void) {
    std::vector<int> iv, iv2;

    iv.assign({1, 2, 3});
    iv2.assign(iv.begin(), iv.end());

    for (std::vector<int>::const_iterator it = iv2.begin(); it != iv2.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;

    std::cout << "max_size: " << iv2.max_size() << std::endl;
    std::cout << "capacity: " << iv2.capacity() << std::endl;
    iv2.push_back(10);
    std::cout << "capacity: " << iv2.capacity() << std::endl;
    iv2.reserve(100);
    std::cout << "capacity: " << iv2.capacity() << std::endl;
    iv2.shrink_to_fit();
    std::cout << "capacity: " << iv2.capacity() << std::endl;

    return 0;
}
