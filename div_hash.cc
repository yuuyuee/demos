#include <iostream>
#include <cmath>

int main(void) {
    for (int i = 0; i < 20; i++) {
        std::cout << "key " << i << ", hash " << (fmod(i * 0.6180339887, 1.0) * pow(2, 14)) << std::endl;
    }
    return 0;
}
