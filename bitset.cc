#include <iostream>
#include <bitset>
int main(void) {
    typedef std::bitset<8> feature;
    feature b0(0xFF);
    feature b1(0x01);
    feature diff = b0 & b1;
    std::cout << diff << std::endl;
    std::cout << diff.size() << std::endl;
    std::cout << diff.count() << std::endl;
    std::cout << (float)diff.count() / (float)diff.size() * 100 << '%' << std::endl;
    diff.reset();
    std::cout << diff << std::endl;
    feature empty;
    if (!empty)
        std::cout << "empty" << std::endl;
    return 0;
}
