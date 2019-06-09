#include <boost/log/trivial.hpp>

int main(void) {
    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "A trace severity message";
    BOOST_LOG_TRIVIAL(info) << "A trace severity message";
    BOOST_LOG_TRIVIAL(warning) << "A trace severity message";
    BOOST_LOG_TRIVIAL(error) << "A trace severity message";
    BOOST_LOG_TRIVIAL(fatal) << "A trace severity message";
    return 0;
}
