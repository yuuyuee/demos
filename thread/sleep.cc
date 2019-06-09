#include <iostream>
#include <thread>
#include <unistd.h>

void thread_call0() {
    sleep(3);
    std::cout << "Call thread_call0()" << std::endl;
}

void thread_call1() {
    sleep(5);
    std::cout << "Call thread_call1()" << std::endl;
}

int main(void) {
    std::thread t0(thread_call0);
    std::thread t1(thread_call1);
    t0.join();
    t1.join();
    return 0;
}
