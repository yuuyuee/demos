#include <cassert>
#include <iostream>
#include <atomic>
#include <string>
#include <thread>

std::atomic<std::string*> ptr(0);
int data = 0;

void producer() {
    std::cout << "Producer thread running." << std::endl;
    std::string* p = new std::string("Hello");
    data = 42;
    ptr.store(p, std::memory_order_release);
}

void consumer() {
    std::cout << "Consumer thread running." << std::endl;
    std::string* p2;
    while (!(p2 = ptr.load(std::memory_order_acquire)))
        std::cout << "{Pointer is empty}" << std::endl;
    assert(*p2 == "Hello");
    assert(data == 42);
}

int main() {
    std::thread tr0(producer), tr1(consumer);
    tr0.join(); tr1.join();
    return 0;
}
