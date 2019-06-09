#include <iostream>
#include <atomic>
#include <thread>
#include <cassert>

std::atomic<bool> x(false), y(false);
std::atomic<int> z(0);

void write_x() {
    x.store(true);
}

void write_y() {
    y.store(true);
}

void read_x_then_y() {
    while (!x.load());
    if (y.load())
        z++;
}

void read_y_then_x() {
    while (!y.load());
    if (x.load())
        z++;
}

int main() {
    std::thread tr0(write_x);
    std::thread tr1(write_y);
    std::thread tr2(read_x_then_y);
    std::thread tr3(read_y_then_x);

    tr0.join();
    tr1.join();
    tr2.join();
    tr3.join();
    std::cout << "x: " << x << ", y: " << y << ", z: " << z << std::endl;

    return 0;
}
