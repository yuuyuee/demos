// X3130 (yuyue) X3130@njsecnet.com
// Changes:
//     2018/05/07 Create

#ifndef __PIEV2_UTILS_H__
#define __PIEV2_UTILS_H__

#ifndef _GNU_SOURCE
#   define _GNU_SOURCE
#endif

#if __cplusplus < 201103L && !defined(__GXX_EXPERIMENTAL_CXX0X__)
#   error "Use -std=c++0x or -std=c++11 options for G++ complier"
#endif

#define GCC_VERSION_AT_LEAST(major, minor)                                  \
    ((__GNUC__ > (major)) || (__GNUC__ == (major) && __GNUC_MINOR__ >= (minor)))

#if !GCC_VERSION_AT_LEAST(4, 6)
#   include <stddef.h>
#   define nullptr NULL
typedef decltype(nullptr) nullptr_t;
#   define constexpr const
#endif

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName)                                  \
    TypeName(const TypeName&) = delete;                                     \
    void operator=(const TypeName&) = delete
#endif

#define PREDICT_TRUE(expr)  __builtin_expect(!!(expr), 1)
#define PREDICT_FALSE(expr) __builtin_expect(!!(expr), 0)

#include <cstdlib>

#define CHECK(cond) do {                                                    \
    if (PREDICT_FALSE(!(cond))) {                                           \
        __builtin_printf("%s:%d: %s: Assert %s failed\n",                   \
                __FILE__, __LINE__, __PRETTY_FUNCTION__, #cond);            \
        __builtin_abort();                                                  \
    }                                                                       \
} while (0)

#define CHECK_EQ(a, b) CHECK((a) == (b))
#define CHECK_NE(a, b) CHECK((a) != (b))
#define CHECK_GT(a, b) CHECK((a) >  (b))
#define CHECK_GE(a, b) CHECK((a) >= (b))
#define CHECK_LT(a, b) CHECK((a) <  (b))
#define CHECK_LE(a, b) CHECK((a) <= (b))

#include <string>
#include <vector>
#include <type_traits>
#include <memory>

namespace utils {
// constant, function, description
#define MAKE_ERROR_CODE(XXX)                                                \
    XXX(CANCELLED,           Cancelled,          "Cancelled")               \
    XXX(INVALID_ARGUMENT,    InvalidArgument,    "Invalid argument")        \
    XXX(NOT_FOUND,           NotFound,           "Not Found")               \
    XXX(ALREADY_EXISTS,      AlreadyExists,      "Already exists")          \
    XXX(RESOURCE_EXHAUSTED,  ResourceExhausted,  "Resource exhausted")      \
    XXX(UNAVAILABLE,         Unavailable,        "Unavailable")             \
    XXX(FAILED_PRECONDITION, FailedPrecondition, "Failed precondition")     \
    XXX(OUT_OF_RANGE,        OutOfRange,         "Out of range")            \
    XXX(UNIMPLEMENTED,       Unimplemented,      "Unimplemented")           \
    XXX(INTERNAL,            Internal,           "Internal")                \
    XXX(ABORTED,             Aborted,            "Aborted")                 \
    XXX(DEADLINE_EXCEEDED,   DeadlineExceeded,   "Deadline exceeded")       \
    XXX(DATA_LOSS,           DataLoss,           "Data loss")               \
    XXX(PERMISSION_DENIED,   PermissionDenied,   "Permission denied")       \
    XXX(UNAUTHENTICATED,     Unauthenticated,    "Unauthenticated")         \
    XXX(UNKNOWN,             Unknown,            "Unknown")

namespace error {
enum Code {
    OK = 0,
#define XXX(CONST, _1, _2) CONST,
    MAKE_ERROR_CODE(XXX)
#undef XXX
};
} // namespace error

class Status {
public:
    Status(): code_(error::OK), msg_() {}
    Status(error::Code code, std::string msg): code_(code), msg_(msg) {
        CHECK_NE(code, error::OK);
    }

    ~Status() = default;

    bool ok() const { return code_ == error::OK; }
    error::Code code() const { return code_; }
    const std::string& message() const { return msg_; }

    Status(const Status& o): code_(o.code_), msg_(o.msg_) {}
    const Status& operator=(const Status& o) {
        if (this != &o) {
            code_ = o.code_;
            msg_ = o.msg_;
        }
        return *this;
    }

    bool operator==(const Status& o) const { return code_ == o.code_ || msg_ == o.msg_; }
    bool operator!=(const Status& o) const { return !(*this == o); }

    void Update(const Status& new_status) {
        if (this->ok())
            *this = new_status;
    }

    void Clear() { *this = Status(); }
    const std::string ToString() const;

    static const Status OK() { return Status(); }
#define XXX(CONST, FUNC, _2)                                                        \
    static Status FUNC(const std::string msg) { return Status(error::CONST, msg); } \
    static bool Is##FUNC(const Status& o) { return o.code() == error::CONST; }      \
    bool Is##FUNC() const { return code() == error::CONST; }
    MAKE_ERROR_CODE(XXX)
#undef XXX

private:
    error::Code code_;
    std::string msg_;
};

#define RETURN_IF_ERROR(ops) do {                                               \
    Status _status = (ops);                                                     \
    if (!_status.ok())                                                          \
        return _status;                                                         \
} while (0)

Status IOError(int error_num, const std::string& context);

// An interface that defines input streaming operations.
class InputStream {
public:
    InputStream() = default;
    virtual ~InputStream() = default;

    // Reads the next n bytes from the source to result. Typical return codes:
    // Status::OK()         - in case of success.
    // Status::OutOfRange() - not enough bytes remaining before end of file.
    // Other                - in case of failed.
    virtual Status Read(size_t n, std::string* result) const = 0;

private:
    DISALLOW_COPY_AND_ASSIGN(InputStream);
};

// An interface that defines output streaming operations.
class OutputStream {
public:
    OutputStream() = default;
    virtual ~OutputStream() = default;

    // Appends the data to source. Typical return codes:
    // Status::OK() - in case of success.
    // Other        - in case of failed.
    virtual Status Append(const std::string& data) = 0;

private:
    DISALLOW_COPY_AND_ASSIGN(OutputStream);
};

class FileInputStream: public InputStream {
public:
    FileInputStream(const std::string filename, int fd, bool owns_fd = false);
    ~FileInputStream();

    Status Read(size_t n, std::string* result) const;

private:
    class FileInputStreamImpl;
    std::unique_ptr<FileInputStreamImpl> impl_;
    DISALLOW_COPY_AND_ASSIGN(FileInputStream);
};

class FileOutputStream: public OutputStream {
public:
    FileOutputStream(const std::string filename, int fd, bool owns_fd = false);
    ~FileOutputStream();

    Status Append(const std::string& data);

private:
    class FileOutputStreamImpl;
    std::unique_ptr<FileOutputStreamImpl> impl_;
    DISALLOW_COPY_AND_ASSIGN(FileOutputStream);
};

class StringInputStream: public InputStream {
public:
    StringInputStream(const char* s, size_t n);
    StringInputStream(const std::string s);
    ~StringInputStream();

    Status Read(size_t n, std::string* result) const;

private:
    class StringInputStreamImpl;
    std::unique_ptr<StringInputStreamImpl> impl_;
    DISALLOW_COPY_AND_ASSIGN(StringInputStream);
};

class StringOutputStream: public OutputStream {
public:
    explicit StringOutputStream(std::string* const s);
    ~StringOutputStream();

    Status Append(const std::string& data);

private:
    class StringOutputStreamImpl;
    std::unique_ptr<StringOutputStreamImpl> impl_;
    DISALLOW_COPY_AND_ASSIGN(StringOutputStream);
};

struct FileStat {
    int64_t size;
    int64_t mtime_nsec;
    bool is_directory;

    FileStat(): size(-1), mtime_nsec(0), is_directory(false) {}
    FileStat(int64_t size, int64_t mtime_nsec, bool is_directory)
        : size(size), mtime_nsec(mtime_nsec), is_directory(is_directory) {}
    ~FileStat() = default;
};

class FileSystem {
public:
    FileSystem() = default;
    ~FileSystem() = default;

    static bool IsAbsolutePath(const std::string path) {
        return !path.empty() && path[0] == '/';
    }
    static std::string Dirname(const std::string path);
    static std::string Basename(const std::string path);
    static std::string Extension(const std::string path);
    static Status RealPath(const std::string path, std::string* abs);
    static Status FileExists(const std::string filename);
    static Status ScanDir(const std::string path, std::vector<std::string>* result);
    static Status Stat(const std::string filename, FileStat* stats);
    static Status FileSize(const std::string filename, size_t* size);
    static Status DeleteFile(const std::string filename);
    static Status CreateDir(const std::string path);
    static Status RecursivelyCreateDir(const std::string path);
    static Status IsDirectory(const std::string path);
    static Status DeleteDir(const std::string path);
    static Status RecursivelyDeleteDir(const std::string path);
    static Status RenameFile(const std::string src, const std::string target);
    static Status CopyFile(const std::string src, const std::string target);
    static inline Status MoveFile(const std::string src, const std::string target) {
        RETURN_IF_ERROR(CopyFile(src, target));
        return DeleteFile(src);
    }

    static Status NewFileInputStream(const std::string filename, std::unique_ptr<InputStream>* result);
    static Status NewFileOutputStream(const std::string filename, std::unique_ptr<OutputStream>* result);
};

// It is based on the Data Encryption Standard algorithm.
class Crypto {
public:
    // key is a user's typed password and that will be used as the origin salt.
    // key length most 7 bytes or less than.
    Crypto(const std::string key);
    ~Crypto();

    // Encrypt/Decrypt the streams that inputs from in and outputs to out.
    // return Status::OK() in case of success, otherwise failed.
    Status Encrypt(const InputStream* in, OutputStream* out);
    Status Decrypt(const InputStream* in, OutputStream* out);
    const std::string key() const { return key_; }

private:
    std::string key_;
    class CryptoImpl;
    std::unique_ptr<CryptoImpl> impl_;
    DISALLOW_COPY_AND_ASSIGN(Crypto);
};

inline Status EncryptString(const std::string key, const std::string sin, std::string* sout) {
    StringInputStream in(sin);
    StringOutputStream out(sout);
    return Crypto(key).Encrypt(&in, &out);
}

inline Status DecryptString(const std::string key, const std::string sin, std::string* sout) {
    StringInputStream in(sin);
    StringOutputStream out(sout);
    return Crypto(key).Decrypt(&in, &out);
}

inline Status EncryptFile(const std::string key, const std::string fin, std::string fout) {
    std::unique_ptr<InputStream> in;
    RETURN_IF_ERROR(FileSystem::NewFileInputStream(fin, &in));
    std::unique_ptr<OutputStream> out;
    RETURN_IF_ERROR(FileSystem::NewFileOutputStream(fout, &out));
    return Crypto(key).Encrypt(in.get(), out.get());
}

inline Status DecryptFile(const std::string key, const std::string fin, std::string fout) {
    std::unique_ptr<InputStream> in;
    RETURN_IF_ERROR(FileSystem::NewFileInputStream(fin, &in));
    std::unique_ptr<OutputStream> out;
    RETURN_IF_ERROR(FileSystem::NewFileOutputStream(fout, &out));
    return Crypto(key).Decrypt(in.get(), out.get());
}

inline Status EncryptStringToFile(const std::string key, const std::string sin, std::string fout) {
    StringInputStream in(sin);
    std::unique_ptr<OutputStream> out;
    RETURN_IF_ERROR(FileSystem::NewFileOutputStream(fout, &out));
    return Crypto(key).Encrypt(&in, out.get());
}

inline Status DecryptStringToFile(const std::string key, const std::string sin, std::string fout) {
    StringInputStream in(sin);
    std::unique_ptr<OutputStream> out;
    RETURN_IF_ERROR(FileSystem::NewFileOutputStream(fout, &out));
    return Crypto(key).Decrypt(&in, out.get());
}

inline Status EncryptFileToString(const std::string key, const std::string fin, std::string* sout) {
    std::unique_ptr<InputStream> in;
    RETURN_IF_ERROR(FileSystem::NewFileInputStream(fin, &in));
    StringOutputStream out(sout);
    return Crypto(key).Encrypt(in.get(), &out);
}

inline Status DecryptFileToString(const std::string key, const std::string fin, std::string* sout) {
    std::unique_ptr<InputStream> in;
    RETURN_IF_ERROR(FileSystem::NewFileInputStream(fin, &in));
    StringOutputStream out(sout);
    return Crypto(key).Decrypt(in.get(), &out);
}

class ZlibDeflate {
public:
    ZlibDeflate();
    ~ZlibDeflate();

    // Compresses the input from in to out, and stops when the input buffer becomes empty.
    // return status::OK() in case of success, otherwise failed.
    Status operator()(const InputStream* in, OutputStream* out);

private:
    class ZlibDeflateImpl;
    std::unique_ptr<ZlibDeflateImpl> impl_;
    DISALLOW_COPY_AND_ASSIGN(ZlibDeflate);
};

class ZlibInflate {
public:
    ZlibInflate();
    ~ZlibInflate();

    // Uncompresses the input from in to out, and stops when the input buffer becomes empty.
    // return status::OK() in case of success, otherwise failed.
    Status operator()(const InputStream* in, OutputStream* out);

private:
    class ZlibInflateImpl;
    std::unique_ptr<ZlibInflateImpl> impl_;
    DISALLOW_COPY_AND_ASSIGN(ZlibInflate);
};

inline Status DeflateString(const std::string sin, std::string* sout) {
    StringInputStream in(sin);
    StringOutputStream out(sout);
    return ZlibDeflate().operator()(&in, &out);
}

inline Status InflateString(const std::string sin, std::string* sout) {
    StringInputStream in(sin);
    StringOutputStream out(sout);
    return ZlibInflate().operator()(&in, &out);
}

inline Status DeflateFile(const std::string fin, std::string fout) {
    std::unique_ptr<InputStream> in;
    RETURN_IF_ERROR(FileSystem::NewFileInputStream(fin, &in));
    std::unique_ptr<OutputStream> out;
    RETURN_IF_ERROR(FileSystem::NewFileOutputStream(fout, &out));
    return ZlibDeflate().operator()(in.get(), out.get());
}

inline Status InflateFile(const std::string fin, std::string fout) {
    std::unique_ptr<InputStream> in;
    RETURN_IF_ERROR(FileSystem::NewFileInputStream(fin, &in));
    std::unique_ptr<OutputStream> out;
    RETURN_IF_ERROR(FileSystem::NewFileOutputStream(fout, &out));
    return ZlibInflate().operator()(in.get(), out.get());
}

inline Status DeflateStringToFile(const std::string sin, std::string fout) {
    StringInputStream in(sin);
    std::unique_ptr<OutputStream> out;
    RETURN_IF_ERROR(FileSystem::NewFileOutputStream(fout, &out));
    return ZlibDeflate().operator()(&in, out.get());
}

inline Status InflateStringToFile(const std::string sin, std::string fout) {
    StringInputStream in(sin);
    std::unique_ptr<OutputStream> out;
    RETURN_IF_ERROR(FileSystem::NewFileOutputStream(fout, &out));
    return ZlibInflate().operator()(&in, out.get());
}

inline Status DeflateFileToString(const std::string fin, std::string* sout) {
    std::unique_ptr<InputStream> in;
    RETURN_IF_ERROR(FileSystem::NewFileInputStream(fin, &in));
    StringOutputStream out(sout);
    return ZlibDeflate().operator()(in.get(), &out);
}

inline Status InflateFileToString(const std::string fin, std::string* sout) {
    std::unique_ptr<InputStream> in;
    RETURN_IF_ERROR(FileSystem::NewFileInputStream(fin, &in));
    StringOutputStream out(sout);
    return ZlibInflate().operator()(in.get(), &out);
}

namespace internal {
bool Strtol(std::string str, long int* result, int base);
}

template<typename Tp>
inline bool StrtoInt(std::string str, Tp* result, int base = 10) {
    static_assert(std::is_integral<Tp>::value, "template argument substitution failed");
    long int value = 0;
    bool ret = internal::Strtol(str, &value, base);
    if (!ret)
        return false;
    *result = static_cast<Tp>(value);
    return true;
}

class License {
public:
    License(std::string cipher);
    ~License();

    // Generator the license cipher by the @identify, @expired and output to @cipher.
    // @identify Host identify string most 16 bytes or less than.
    // @date expired date, format is Year/Mon/Day, like as the "2018/4/30",
    // in case of otherwise return Status::InvalidArgument().
    // return Status::OK() if success and cipher will be returned by @cipher.
    static Status Generator(const std::string identify, const uint64_t magic,
                            const std::string date, std::string* cipher);

    const std::string& Identify() const;
    const uint64_t Magic() const;
    const std::string Date() const;

    bool IsValid() const;
    bool IsExpired() const;

private:
    class LicenseImpl;
    std::shared_ptr<LicenseImpl> impl_;
};
} // namespace utils
#endif // __PIEV2_UTILS_H__
