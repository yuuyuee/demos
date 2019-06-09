#include <iostream>
#include <cstddef>
#include <string>
#include <utility>

#include <vector>
#include <boost/log/attributes/value_extraction.hpp>
#include <boost/log/attributes/attribute_value_impl.hpp>
#include <boost/log/utility/value_ref.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;

void print_value(const logging::attribute_value& attr) {
    logging::value_ref<int> val = logging::extract<int>(attr);
    if (val) {
        std::cout << "Extraction successed: " << val.get() << std::endl;
    } else {
        std::cout << "Extraction failed" << std::endl;
    }
}

int main(int, char**) {
    print_value(attrs::make_attribute_value(10));
    return 0;
}
