#include <iostream>
#include <boost/thread.hpp>

// boost.thread classes
// 1. ::boost::thread
// 2. ::boost::thread::attributes


void thread_call() {
    std::cout << "New thread" << std::endl;
}

int main(void) {
    ::boost::thread thread(thread_call);

    thread.join();

    return 0;
}
