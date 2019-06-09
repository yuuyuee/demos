#include <iostream>
#include <array>

int main(void) {
    std::array<int, 3> ia = {1, 2, 3};

    std::cout << "size: " << ia.size() << std::endl;
    std::cout << "max_size: " << ia.max_size() << std::endl;
    for (int i = 0; i < 3; i++)
        std::cout << ia[i] << " ";
    std::cout << std::endl;

    return 0;
}
