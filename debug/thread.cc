#include <iostream>
#include <thread>
#include <mutex>

std::mutex lock0;
std::mutex lock1;

void func0() {
    std::lock_guard<std::mutex> guard(lock0);
    while (1);
}

void func1() {
    std::lock_guard<std::mutex> guard(lock1);
    while (1);
}

int main(void) {
    std::thread tr0(func0);
    std::thread tr1(func1);
    tr0.join();
    tr1.join();

    return 0;
}
