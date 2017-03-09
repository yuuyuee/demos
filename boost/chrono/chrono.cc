#include <iostream>
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

    //time_pointer

    return 0;
}
