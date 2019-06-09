#include <iostream>
#include <mutex>
#include <thread>

void init() {
    std::cout << "Initialize" << std::endl;
}

std::once_flag flag;

void tr() {
    std::call_once(flag, init);
}

int main(void) {
    std::thread t0(tr), t1(tr), t2(tr), t3(tr);
    t0.join();
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
