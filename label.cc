#include <iostream>
#include <unistd.h>

int main() {
    const void* const label = &&label_0;
label_0:
    std::cout << "hello world" << std::endl;
    sleep(2);
    goto *label;

    return 0;
}
