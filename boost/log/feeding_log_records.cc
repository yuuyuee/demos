#include <boost/log/trivial.hpp>
#include <boost/log/core/core.hpp>
#include <boost/log/core/record.hpp>
#include <boost/log/core/record_view.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

namespace logging = boost::log;

int main(void) {
    logging::attribute_set attrs;
    boost::shared_ptr<logging::core> core = logging::core::get();
    logging::record rec = core->open_record(attrs);
    if (rec) {
        logging::record_ostream strm(rec);
        strm << "hello world!";
        strm.flush();
        core->push_record(boost::move(rec));
    }

    return 0;
}
