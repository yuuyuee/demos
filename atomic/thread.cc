#include <iostream>
#include <thread>

void thread_main() {
    std::cout << "Call thread_main()" << std::endl;
}

int main(void) {
    std::thread t(thread_main);
    t.join();
    std::cout << std::thread::hardware_concurrency() << std::endl;
    return 0;
}
