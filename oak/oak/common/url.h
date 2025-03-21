// Copyright RCT Power 2025

#ifndef RCTEMS_COMMON_URL_H_
#define RCTEMS_COMMON_URL_H_

#include <string>
#include <map>
#include <string_view>

namespace rctems {
namespace common {

// This class implemented the  Uniform Resource Identifier (URI).
// [scheme:][user_info@][host][:port][path][?query][#fragment]
class Url {
 public:
    Url();
    bool Parse(std::string_view url);
    std::string Normalize() const;

    const std::string& Scheme() const;
    const std::string& User() const;
    const std::string& Password() const;
    const std::string& Host() const;
    const std::string& Port() const;
    const std::string& Path() const;
    const std::map<std::string, std::string>& Query() const;
    const std::string& Query(const std::string& key) const;
    const std::string& Fragment() const;

 private:
    std::string scheme_;
    std::string user_;
    std::string password_;
    std::string host_;
    std::string host_ipv6_;
    std::string port_;
    std::string path_;
    std::map<std::string, std::string> query_;
    std::string fragment_;
};

}  // namespace common
}  // namespace rctems

#endif  // RCTEMS_COMMON_URL_H_
