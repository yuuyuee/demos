// Copyright RCT Power 2025

#include "url.h"

#include <cstring>
#include <cctype>
#include <cassert>
#include <locale>
#include <string>
#include <map>
#include <iterator>
#include <algorithm>
#include <string_view>

namespace rctems {
namespace common {
namespace {

inline std::string_view TrimFront(std::string_view str) {
    for (size_t i = 0; i < str.size(); ++i) {
        if (!isspace(str[i]))
            return str.substr(i);
    }
    return std::string_view();
}

inline std::string_view TrimBack(std::string_view str) {
    for (size_t i = str.size(); i > 0; --i) {
        if (!isspace(str[i - 1]))
            return str.substr(0, i);
    }
    return std::string_view();
}

inline std::string_view Trim(std::string_view str) {
    return TrimFront(TrimBack(str));
}

inline bool IsUnreserved(char c) {
    // Unreserved characters: ALPA DIGIT -._~
    return (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            c == '-' || c == '.' || c == '_' || c == '~';
}

inline char HexToLetter(char c) {
    if ((c >= 0) && (c < 10)) {
      return c + '0';
    } else if ((c >= 10) && (c < 16)) {
      return c - 10 + 'A';
    }
    return c;
}

inline char LetterToHex(char c) {
    if ((c >= '0') && (c <= '9')) {
      return c - '0';
    } else if ((c >= 'a') && (c <= 'f')) {
      return c + 10 - 'a';
    } else if ((c >= 'A') && (c <= 'F')) {
      return c + 10 - 'A';
    }
    return c;
}

inline void ToLower(std::string* str) {
    std::transform(str->cbegin(), str->cend(), str->begin(), tolower);
}

std::string PercentEncode(std::string_view str) {
    std::string res;
    for (auto c : str) {
        if (IsUnreserved(c)) {
            res.push_back(c);
        } else {
            res.push_back('%');
            res.push_back(HexToLetter((c >> 4) & 0x0F));
            res.push_back(HexToLetter(c & 0x0F));
        }
    }
    return res;
}

bool PercentDecode(std::string_view str, std::string* res) {
    std::string res_;
    for (auto it = str.cbegin(); it != str.cend();) {
        if (*it == '%') {
            if (str.cend() - it < 3)
                return false;
            ++it;
            char c = LetterToHex(*it) << 8;
            ++it;
            c |= LetterToHex(*it);
            ++it;
            res_.push_back(c);
        } else if (*it == '+') {
            res_.push_back(' ');
            ++it;
        } else {
            res_.push_back(*it);
            ++it;
        }
    }
    res->swap(res_);
    return true;
}

inline bool ValidScheme(std::string_view scheme) {
    auto it = std::find_if(scheme.cbegin(), scheme.cend(),
            [] (char c) {
                return !std::isalnum(c) && c != '+' && c != '-' && c != '.';
            });
    return it == scheme.cend();
}

bool ParseAuthority(std::string_view auth,
                    std::string_view* user,
                    std::string_view* password,
                    std::string_view* host,
                    std::string_view* host_ipv6,
                    std::string_view* port) {
    assert(!auth.empty());
    auto pos = auth.find('@');
    if (pos != std::string_view::npos) {
        std::string_view user_pass = auth.substr(0, pos);
        auth.remove_prefix(pos + 1);
        if (auth.empty())
            return false;
        pos = user_pass.find(':');
        if (pos != std::string_view::npos)
            *password = user_pass.substr(pos + 1);
        *user = user_pass.substr(0, pos);
    }

    if (auth.front() == '[') {
        pos = auth.find_first_of(']');
        if (pos == std::string_view::npos)
            return false;
        *host_ipv6 = auth.substr(1, pos - 1);
        auth.remove_prefix(pos + 1);
        if (!auth.empty()) {
            if (auth.front() != ':')
                return false;
            *port = auth;
        }
    } else {
        pos = auth.find(':');
        if (pos != std::string_view::npos) {
            *port = auth.substr(pos + 1);
            auth.remove_suffix(auth.size() - pos);
        }
        *host = auth;
    }

    return true;
}

inline bool ParseQuery(std::string_view query,
                       std::map<std::string, std::string>* dict) {
    std::map<std::string, std::string> res;
    std::string key, val;
    while (!query.empty()) {
        auto pos = query.find('&');
        std::string_view kv = query.substr(0, pos);
        if (pos == std::string_view::npos) {
            pos = query.size();
        } else {
            ++pos;
        }
        query.remove_prefix(pos);

        pos = kv.find('=');
        if (pos == std::string_view::npos)
            return false;
        std::string_view left = kv.substr(0, pos);
        key.clear();
        if (!PercentDecode(left, &key))
            return false;
        std::string_view right = kv.substr(pos + 1);
        val.clear();
        if (!PercentDecode(right, &val))
            return false;
        res[key] = val;
    }
    dict->swap(res);
    return true;
}


}  // anonymous namespace

Url::Url() {}

// [scheme:][user_info@][host][:port][path][?query][#fragment]
// Generic delimiters: :/@[]?#
// Sub delimiters: !$&'()*+,;=
bool Url::Parse(std::string_view url) {
    url = Trim(url);
    if (url.empty())
        return false;
    std::string buf(url.data(), url.size());
    url = buf;

    auto pos = url.find("://");
    if (pos != std::string_view::npos) {
        auto scheme = url.substr(0, pos);
        if (!ValidScheme(scheme))
            return false;
        this->scheme_ = static_cast<std::string>(scheme);
        ToLower(&this->scheme_);
        url.remove_prefix(pos + 3);
        if (url.empty())
            return false;
    }

    pos = url.find_first_of("/?#");
    std::string_view auth = url.substr(0, pos);
    if (!auth.empty()) {
        std::string_view host, host_ipv6, port, user, password;
        if (!ParseAuthority(auth, &user, &password, &host, &host_ipv6, &port))
            return false;
        if (!PercentDecode(host, &this->host_) ||
                !PercentDecode(host_ipv6, &this->host_ipv6_) ||
                !PercentDecode(port, &this->port_) ||
                this->port_.find_first_not_of("0123456789") != std::string::npos ||
                !PercentDecode(user, &this->user_) ||
                !PercentDecode(password, &this->password_)) {
            return false;
        }
        ToLower(&this->host_);
        ToLower(&this->host_ipv6_);
    }

    if (pos != std::string_view::npos) {
        url.remove_prefix(pos);
        while (!url.empty()) {
            if (url.front() == '/') {
                pos = url.find_first_of("?#");
                auto path = url.substr(0, pos);
                if (!PercentDecode(path, &this->path_))
                    return false;
                // ToLower(&this->path_);
                if (pos == std::string_view::npos)
                    pos = url.size();
                url.remove_prefix(pos);
            } else if (url.front() == '?') {
                url.remove_prefix(1);
                pos = url.find('#');
                auto qstr = url.substr(0, pos);
                if (!qstr.empty() && !ParseQuery(qstr, &this->query_))
                    return false;
                if (pos == std::string_view::npos)
                    pos = url.size();
                url.remove_prefix(pos);
            } else if (url.front() == '#') {
                url.remove_prefix(1);
                if (!PercentDecode(url, &this->fragment_))
                    return false;
                url.remove_prefix(url.size());
            }
        }
    }

    return !this->host_.empty() || !this->host_ipv6_.empty() || !this->path_.empty();
}

std::string Url::Normalize() const {
    // TODO(Ivan): implement
    std::string res;
    if (!this->scheme_.empty()) {
        res.append(this->scheme_);
        res.append("://");
    }

    if (!this->host_.empty()) {
        if (!this->user_.empty()) {
            res.append(PercentEncode(this->user_));
            if (!this->password_.empty()) {
                res.push_back(':');
                res.append(PercentEncode(this->password_));
            }
            res.push_back('@');
        }
        res.append(this->host_);
        if (!this->port_.empty()) {
            res.push_back(':');
            res.append(this->port_);
        }
    } else if (!this->host_ipv6_.empty()) {
        if (!this->user_.empty()) {
            res.append(this->user_);
            if (!this->password_.empty()) {
                res.push_back(':');
                res.append(this->password_);
            }
            res.push_back('@');
        }
        res.push_back('[');
        res.append(this->host_ipv6_);
        res.push_back(']');
        if (!this->port_.empty()) {
            res.push_back(':');
            res.append(this->port_);
        }
    }

    if (!this->path_.empty())
        res.append(this->path_);

    if (!this->query_.empty()) {
        char lead_c = '?';
        for (auto it : this->query_) {
            res.push_back(lead_c);
            res.append(PercentEncode(it.first));
            res.push_back('=');
            res.append(PercentEncode(it.second));
            if (lead_c != '&')
                lead_c = '&';
        }
    }

    if (!this->fragment_.empty()) {
        res.push_back('#');
        res.append(PercentEncode(this->fragment_));
    }
    return res;
}

const std::string& Url::Scheme() const {
    return this->scheme_;
}

const std::string& Url::User() const {
    return this->user_;
}

const std::string& Url::Password() const {
    return this->password_;
}

const std::string& Url::Host() const {
    return !this->host_.empty() ? this->host_ : this->host_ipv6_;
}

const std::string& Url::Port() const {
    return this->port_;
}

const std::string& Url::Path() const {
    return this->path_;
}

const std::map<std::string, std::string>& Url::Query() const {
    return this->query_;
}

const std::string& Url::Query(const std::string& key) const {
    static const std::string empty_str;
    auto const it = this->query_.find(key);
    if (it != this->query_.cend())
        return empty_str;
    return it->second;
}

const std::string& Url::Fragment() const {
    return this->fragment_;
}

}  // namespace common
}  // namespace rctems


#ifdef RCTEMS_COMMON_URL_TEST
#include <cassert>
#include <iostream>

#define ASSERT_VALID_URL(s) do {                                            \
    rctems::common::Url url;                                                \
    bool res = url.Parse(s);                                                \
    std::cout << "URL: " << s << std::endl;                                 \
    std::cout << "URL normalize: " << url.Normalize() << std::endl;         \
    std::cout << "Scheme: " << url.Scheme() << std::endl;                   \
    std::cout << "User: " << url.User() << std::endl;                       \
    std::cout << "Password: " << url.Password() << std::endl;               \
    std::cout << "Host: " << url.Host() << std::endl;                       \
    std::cout << "Port: " << url.Port() << std::endl;                       \
    std::cout << "Path: " << url.Path() << std::endl;                       \
    auto const& dict = url.Query();                                         \
    std::cout << "Query: " << std::endl;                                    \
    for (auto it = dict.cbegin(); it != dict.cend(); ++it)                  \
        std::cout << "  " << it->first << " = " << it->second << std::endl; \
    std::cout << "Fragment: " << url.Fragment() << std::endl;               \
    std::cout << std::endl;                                                 \
    assert(res && #s);                                                      \
} while (0)

int main() {
    ASSERT_VALID_URL("TCP://user:pass@EXAMPLE.com:8800/path?k=v#frag");
    ASSERT_VALID_URL("tcp://user:@example.com:8800/path?k=v#frag");
    ASSERT_VALID_URL("tcp://user@example.com:8800/path?k=v#frag");
    ASSERT_VALID_URL("tcp://user@example.com:8800/path?k=v#");
    ASSERT_VALID_URL("tcp://user@example.com:8800/path?");
    ASSERT_VALID_URL("tcp://user@example.com:8800/path");
    ASSERT_VALID_URL("tcp://user@example.com/path?");
    ASSERT_VALID_URL("tcp://user@example.com/");
    ASSERT_VALID_URL("tcp://example.com");
    ASSERT_VALID_URL("tcp://127.0.0.1");
    ASSERT_VALID_URL("tcp://[::1]");

    ASSERT_VALID_URL("user:pass@example.com:8800/path?k=v#frag");
    ASSERT_VALID_URL("/path?k=v#frag");
    ASSERT_VALID_URL("/path?k=v#");
    ASSERT_VALID_URL("/path?k=v");
    ASSERT_VALID_URL("/path?");
    ASSERT_VALID_URL("/path?k=v#frag");
    ASSERT_VALID_URL("file:///path?k=v#frag");

    return 0;
}
#endif
