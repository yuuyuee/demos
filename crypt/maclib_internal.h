// X3130 (yuyue) X3130@njsecnet.com
// Changes:
//     2018/05/07 Create

#ifndef __PIEV2_MACLIB_INTERNAL_H__
#define __PIEV2_MACLIB_INTERNAL_H__

#include <string>
#include <list>
#include <unordered_map>
#include <map>

#include "utils.h"
#include "maclib.h"
#if 0
#   include "oui.h"
#else
#define MAKE_MAC_VENDORS(XXX) \
    XXX(0xE043DBUL, "Shenzhen ViewAt Technology Co.,Ltd") \
    XXX(0xE043DBUL, "Shenzhen ViewAt Technology Co.,Ltd") \
    XXX(0xE043DBUL, "Shenzhen ViewAt Technology Co.,Ltd") \
    XXX(0xE043DBUL, "Shenzhen ViewAt Technology Co.,Ltd") \
    XXX(0xE043DBUL, "Shenzhen ViewAt Technology Co.,Ltd") \
    XXX(0xE043DBUL, "Shenzhen ViewAt Technology Co.,Ltd")
#endif

namespace maclib {
class DatabaseOps {
public:
    DatabaseOps();
    ~DatabaseOps();

    const utils::Status Load(const std::string path);
    const utils::Status Store(const std::string path);
    const utils::Status Update();
};

struct MAC {
    uint64_t value;
    std::list<uint64_t> phones;
    std::list<uint64_t> imsi;
    std::list<uint64_t> imei;
    unsigned int flags;
};

struct Phone {
    uint64_t value;
    std::list<uint64_t> mac;
};

class Record {
public:
    Record();
    ~Record();
};

class Database {
public:
    typedef std::map<uint64_t, MAC> MACTable;
    typedef std::map<uint64_t, Phone> PhoneTable;

    Database();
    ~Database();

    const utils::Status Add();
    const utils::Status Remove();
    const utils::Status Modify();
    const utils::Status Find();

private:

};




class MACViewer::MACViewerImpl {
public:
    MACViewerImpl(const std::string& file): file_(file) {}
    ~MACViewerImpl() = default;

    MACInfo Find(const char mac[12]) const { return MACInfo("test", 0, 0); }

private:
    std::string file_;
    DISALLOW_COPY_AND_ASSIGN(MACViewerImpl);
};

MACViewer::MACViewer(const std::string file): impl_(new MACViewerImpl(file)) {}

MACInfo MACViewer::Find(const char mac[12]) const { return impl_->Find(mac); }

} // namespace maclib
#endif // __PIEV2_MACLIB_INTERNAL_H__
