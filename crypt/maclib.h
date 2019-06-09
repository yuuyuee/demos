// X3130 (yuyue) X3130@njsecnet.com
// Changes:
//     2018/05/07 Create

#ifndef __PIEV2_MACLIB_H__
#define __PIEV2_MACLIB_H__

#ifndef _GNU_SOURCE
#   define _GNU_SOURCE
#endif

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName)                                  \
    TypeName(const TypeName&) = delete;                                     \
    void operator=(const TypeName&) = delete
#endif

#include <string>
#include <memory>

namespace maclib {
enum MACCapture {
    TERMINAL_MAC = 1u,
    ROUTER_MAC = 2u,
    UNKNOWN_MAC = 4u,
};

struct MACInfo {
    MACInfo(const std::string name, uint64_t value, unsigned int attr)
        : name(name), value(value), attributes(attr) {}
    ~MACInfo() = default;

    std::string name;
    uint64_t value;
    unsigned int attributes;
};

class MACViewer {
public:
    MACViewer(const std::string file);
    ~MACViewer() = default;

    MACInfo Find(const char mac[12]) const;

private:
    struct MACViewerImpl;
    std::unique_ptr<MACViewerImpl> impl_;
    DISALLOW_COPY_AND_ASSIGN(MACViewer);
};

} // namespace maclib
#endif // __PIEV2_MACLIB_H__
