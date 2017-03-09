#include <iostream>
#include <unistd.h>
#include <boost/chrono.hpp>

namespace chrono = ::boost::chrono;

int main(void) {
    // duration test
    chrono::seconds sec_zero = chrono::seconds::zero();
    chrono::seconds sec_min = chrono::seconds::min();
    chrono::seconds sec_max = chrono::seconds::max();
    std::cout << "seconds zero: " << sec_zero.count() << std::endl;
    std::cout << "seconds min: " << sec_min.count() << std::endl;
    std::cout << "seconds max: " << sec_max.count() << std::endl;
    sec_zero += chrono::minutes(1);
    chrono::minutes min = chrono::duration_cast<chrono::minutes>(sec_zero);
    std::cout << "second "
              << sec_zero.count()
              << " convert minutes "
              << min.count()
              << std::endl;

    //time_point
    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    sleep(2);
    chrono::time_point<chrono::system_clock> stop = chrono::system_clock::now();
    chrono::time_point<chrono::system_clock> off = stop - start.time_since_epoch();
    //chrono::nanoseconds off = stop - start;

    std::cout << "start point now: " << start.time_since_epoch().count() << std::endl;
    std::cout << "stop point now: " << stop.time_since_epoch().count() << std::endl;
    std::cout << "offset point now: " << off.time_since_epoch().count() << std::endl;
    //std::cout << "offset point now: " << chrono::duration_cast<chrono::seconds>(off).count() << std::endl;
    return 0;
}
