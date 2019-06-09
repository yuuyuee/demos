#include <cstddef>
#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/utility/value_ref.hpp>

namespace logging = boost::log;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

enum severity_level { normal = 0, notification, warning, error, critical };

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)

int main(void) {
    logging::attribute_set attrs;
    attrs.insert("Severity", attrs::make_constant(error));
    logging::record rec = logging::core::get()->open_record(attrs);
    if (rec) {
        logging::attribute_value_set const& values = rec.attribute_values();
        logging::attribute_value_set::const_iterator it = values.find("Severity");
        if (it != values.end()) {
            logging::attribute_value const& value = it->second;
            // Expect that output 3(error) in the here.
            std::cout << value.extract<severity_level>() << std::endl;
        }
    }
}
