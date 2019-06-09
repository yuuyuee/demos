#include <iostream>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>

// boost.thread classes
// 1. ::boost::thread
// 2. ::boost::thread::attributes


void thread_call(const ::boost::thread *thr) {
    ::boost::thread::id id = thr->get_id();
    //::boost::thread::native_handle_type handle = thr->native_handle();
    ::boost::this_thread::sleep_for(::boost::chrono::seconds(2));
    std::cout << "New thread: " << id << std::endl;
    //std::cout << "New thread: " << handle << std::endl;
}

int main(void) {
    ::boost::thread thread0(thread_call, &thread0);
    while (!thread0.joinable()) {
        std::cout << "waiting for join ..." << std::endl;
        ::boost::this_thread::sleep_for(::boost::chrono::seconds(2));
    }
    thread0.join();
    std::cout << "main thread exit" << std::endl;
    return 0;
}
