#include <iostream>
#include <atomic>
#include <thread>
#include <cassert>

#if 0
int main(void) {
    std::atomic<bool> x(false), y(false);
    int z = 0;
    auto f0 = [&] {
        // read y then x
        while (!y.load(std::memory_order_relaxed))
            ;
        if (x.load(std::memory_order_relaxed))
            z++;
    };
    auto f1 = [&] {
        // write x the y
        x.store(true, std::memory_order_relaxed);
        y.store(true, std::memory_order_relaxed);
    };

    std::thread tr0(f1), tr1(f0);
    tr0.join();
    tr1.join();
    assert(z != 0);
    return 0;
}
#else
int main(void) {
    std::atomic<int> x(0);
    auto f = [&] {
        for (int i = 0; i < 1000; i++)
            x.fetch_add(1, std::memory_order_relaxed);
    };

    std::thread tr0(f), tr1(f);
    tr0.join();
    tr1.join();
    std::cout << "x = " << x << std::endl;
    return 0;
}
#endif
