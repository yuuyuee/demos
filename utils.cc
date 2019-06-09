// X3130 (yuyue) X3130@njsecnet.com
// Changes:
//     2018/05/07 Create

#ifndef _GNU_SOURCE
#   define _GNU_SOURCE
#endif

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <dirent.h>
#include <crypt.h>
#include <zlib.h>
#include <sys/time.h>
#include <time.h>

#include <cstdio>
#include <cerrno>
#include <cstring>
#include <algorithm>
#include <utility>

#include <utils.h>

namespace utils {
const std::string Status::ToString() const {
    if (code() == error::OK)
        return "OK";
    const char* type;
    char buffer[30];
    switch (code()) {
#define XXX(CONST, _1, DESC)    \
        case(error::CONST):     \
            type = DESC;        \
            break;
    MAKE_ERROR_CODE(XXX)
#undef XXX
    default:
        snprintf(buffer, sizeof(buffer), "Unknown code(%d)", static_cast<int>(code()));
        type = buffer;
        break;
    }
    std::string result(type);
    result += ": ";
    result += message();
    return result;
}

error::Code ErrnoToCode(int error_num) {
    error::Code code;
    switch (error_num) {
    case 0:
        code = error::OK;
        break;
    case EINVAL:
    case ENAMETOOLONG:
    case E2BIG:
    case EDESTADDRREQ:
    case EDOM:
    case EFAULT:
    case EILSEQ:
    case ENOPROTOOPT:
    case ENOSTR:
    case ENOTSOCK:
    case ENOTTY:
    case EPROTOTYPE:
    case ESPIPE:
        code = error::INVALID_ARGUMENT;
        break;
    case ETIMEDOUT:
    case ETIME:
        code = error::DEADLINE_EXCEEDED;
        break;
    case ENODEV:
    case ENOENT:
    case ENXIO:
    case ESRCH:
        code = error::NOT_FOUND;
        break;
    case EEXIST:
    case EADDRNOTAVAIL:
    case EALREADY:
        code = error::ALREADY_EXISTS;
        break;
    case EPERM:
    case EACCES:
    case EROFS:
        code = error::PERMISSION_DENIED;
        break;
    case ENOTEMPTY:
    case EISDIR:
    case EADDRINUSE:
    case EBADF:
    case EBUSY:
    case ECHILD:
    case EISCONN:
    case ENOTBLK:
    case ENOTCONN:
    case EPIPE:
    case ESHUTDOWN:
    case ETXTBSY:
        code = error::FAILED_PRECONDITION;
        break;
    case ENOSPC:
    case EDQUOT:
    case EMFILE:
    case EMLINK:
    case ENFILE:
    case ENOBUFS:
    case ENODATA:
    case ENOMEM:
    case ENOSR:
    case EUSERS:
        code = error::RESOURCE_EXHAUSTED;
        break;
    case EFBIG:
    case EOVERFLOW:
    case ERANGE:
        code = error::OUT_OF_RANGE;
        break;
    case ENOSYS:
    case ENOTSUP:
    case EAFNOSUPPORT:
    case EPFNOSUPPORT:
    case EPROTONOSUPPORT:
    case ESOCKTNOSUPPORT:
    case EXDEV:
        code = error::UNIMPLEMENTED;
        break;
    case EAGAIN:
    case ECONNREFUSED:
    case ECONNABORTED:
    case ECONNRESET:
    case EINTR:
    case EHOSTDOWN:
    case EHOSTUNREACH:
    case ENETDOWN:
    case ENETRESET:
    case ENETUNREACH:
    case ENOLCK:
    case ENOLINK:
    case ENONET:
        code = error::UNAVAILABLE;
        break;
    case EDEADLK:
    case ESTALE:
        code = error::ABORTED;
        break;
    case ECANCELED:
        code = error::CANCELLED;
        break;
    case EBADMSG:
    case EIDRM:
    case EINPROGRESS:
    case EIO:
    case ELOOP:
    case ENOEXEC:
    case ENOMSG:
    case EPROTO:
    case EREMOTE:
        code = error::UNKNOWN;
        break;
    default:
        code = error::UNKNOWN;
        break;
    }
    return code;
}

Status IOError(int error_num, const std::string& context) {
    error::Code code = ErrnoToCode(error_num);
    if (code == error::UNKNOWN) {
        std::string new_context(context);
        new_context += "; ";
        new_context += strerror(error_num);
        return Status(code, new_context);
    }
    return Status(code, context);
}

const std::string RandomGenerate(unsigned int seed) {
    char buffer[100] = {0};
    snprintf(buffer, 100, "%d", rand_r(&seed));
    return buffer;
}

class FileInputStream::FileInputStreamImpl {
public:
    FileInputStreamImpl(const std::string& filename, int fd, bool owns_fd)
        : filename_(filename), fd_(fd), owns_fd_(owns_fd) {}
    ~FileInputStreamImpl();

    Status Read(size_t n, std::string* result);

private:
    std::string filename_;
    int fd_;
    bool owns_fd_;
    DISALLOW_COPY_AND_ASSIGN(FileInputStreamImpl);
};

FileInputStream::FileInputStreamImpl::~FileInputStreamImpl() {
    if (owns_fd_)
        close(fd_);
}

Status FileInputStream::FileInputStreamImpl::Read(size_t n, std::string* result) {
    Status s;
    if (n > 0) {
        char buffer[n];
        ssize_t have = 0;
        do {
            have = read(fd_, buffer, n);
        } while (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK);
        if (have > 0) {
            if (result->size() != static_cast<size_t>(have))
                result->resize(have);
            std::copy(buffer, buffer + have, result->begin());
        } else if (have == 0) {
            s = Status::OutOfRange("Read less bytes than requested");
        } else {
            s = IOError(errno, filename_);
        }
    }
    return s;
}

FileInputStream::FileInputStream(const std::string& file_name, int fd, bool owns_fd)
    : impl_(new FileInputStreamImpl(file_name, fd, owns_fd)) {}

FileInputStream::~FileInputStream() = default;

Status FileInputStream::Read(size_t n, std::string* result) const {
    return impl_->Read(n, result);
}

class FileOutputStream::FileOutputStreamImpl {
public:
    FileOutputStreamImpl(const std::string& filename, int fd, bool owns_fd)
        : filename_(filename), fd_(fd), owns_fd_(owns_fd) {}
    ~FileOutputStreamImpl();

    Status Append(const std::string& data);

private:
    std::string filename_;
    int fd_;
    bool owns_fd_;
    DISALLOW_COPY_AND_ASSIGN(FileOutputStreamImpl);
};

FileOutputStream::FileOutputStreamImpl::~FileOutputStreamImpl() {
    if (owns_fd_)
        close(fd_);
}

Status FileOutputStream::FileOutputStreamImpl::Append(const std::string& data) {
    ssize_t writed = 0;
    do {
        writed = write(fd_, data.data(), data.size());
    } while (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK);
    Status s;
    if (static_cast<size_t>(writed) != data.size())
        s = IOError(errno, filename_);
    return s;
}

FileOutputStream::FileOutputStream(const std::string& filename, int fd, bool owns_fd)
        : impl_(new FileOutputStreamImpl(filename, fd, owns_fd)) {}

FileOutputStream::~FileOutputStream() = default;

Status FileOutputStream::Append(const std::string& data) {
    return impl_->Append(data);
}

class StringInputStream::StringInputStreamImpl {
public:
    StringInputStreamImpl(const char* s, size_t n): buffer_(s, n), pos_(0) {}
    StringInputStreamImpl(const std::string& s): buffer_(s), pos_(0) {}
    ~StringInputStreamImpl() = default;

    Status Read(size_t n, std::string* result) const;

private:
    std::string buffer_;
    mutable std::string::size_type pos_;
    DISALLOW_COPY_AND_ASSIGN(StringInputStreamImpl);
};

Status StringInputStream::StringInputStreamImpl::Read(size_t n, std::string* result) const {
    result->clear();
    size_t remain = buffer_.size() - pos_;
    Status s;
    if (n > 0) {
        if (remain == 0) {
            s =  Status::OutOfRange("Read less bytes than requested");
        } else {
            size_t min_size = std::min(n, remain);
            result->resize(min_size);
            auto buffer_begin = buffer_.begin() + pos_;
            std::copy(buffer_begin, buffer_begin + min_size, result->begin());
            pos_ += min_size;
        }
    }
    return s;
}

StringInputStream::StringInputStream(const char* s, size_t n)
    : impl_(new StringInputStreamImpl(s, n)) {}

StringInputStream::StringInputStream(const std::string& s)
    : impl_(new StringInputStreamImpl(s)) {}

StringInputStream::~StringInputStream() = default;

Status StringInputStream::Read(size_t n, std::string* result) const {
    return impl_->Read(n, result);
}

class StringOutputStream::StringOutputStreamImpl {
public:
    StringOutputStreamImpl(std::string* s): buffer_(s) {
        buffer_->clear();
    }
    ~StringOutputStreamImpl() = default;

    Status Append(const std::string& data) {
        buffer_->append(data);
        return Status::OK();
    }

private:
    std::string* const buffer_;
    DISALLOW_COPY_AND_ASSIGN(StringOutputStreamImpl);
};

StringOutputStream::StringOutputStream(std::string* const s)
    : impl_(new StringOutputStreamImpl(s)) {}

StringOutputStream::~StringOutputStream() = default;

Status StringOutputStream::Append(const std::string& data) {
    return impl_->Append(data);
}

static inline bool EndWith(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size() && s.substr(s.size() - suffix.size()) == suffix;
}

static std::pair<std::string, std::string> SplitPath(const std::string& path) {
    auto pos = path.rfind('/');
    if (pos == std::string::npos)
        return std::make_pair("", path);
    if (pos == 0)
        return std::make_pair(path.substr(0, 1), path.substr(1));
    return std::make_pair(path.substr(0, pos), path.substr(pos + 1));
}

Status FileSystem::RealPath(const std::string& path, std::string* abs) {
    char resolved_path[PATH_MAX] = {0};
    Status s;
    if (realpath(path.data(), resolved_path) == NULL) {
        s = IOError(errno, path);
    } else {
        *abs = resolved_path;
    }
    return s;
}

Status FileSystem::CurrentDirectory(std::string* path) {
    char buffer[PATH_MAX] = {0};
    Status s;
    if (getcwd(buffer, PATH_MAX) == NULL)
        return IOError(errno, "getcwd");
    *path = buffer;
    return s;
}

std::string FileSystem::Dirname(const std::string& path) {
    return SplitPath(path).first;
}

std::string FileSystem::Basename(const std::string& path) {
    return SplitPath(path).second;
}

std::string FileSystem::Extension(const std::string& path) {
    auto pos = path.rfind('.');
    if (pos == std::string::npos)
        return "";
    return path.substr(pos + 1);
}

std::string FileSystem::Filename(const std::string& path) {
    auto pos = path.rfind('.');
    if (pos == std::string::npos)
        return path;
    return path.substr(0, pos);
}

Status FileSystem::FileExists(const std::string& filename) {
    if (access(filename.data(), F_OK))
        return Status::NotFound(filename);
    return Status::OK();
}

Status FileSystem::ScanDir(const std::string& path, std::vector<std::string>* result) {
    result->clear();
    DIR* d = opendir(path.data());
    if (d == NULL)
        return IOError(errno, path);
    struct dirent* entry;
    while ((entry = readdir(d)) != NULL) {
        std::string name(entry->d_name);
        if (name != "." && name != "..")
            result->push_back(name);
    }
    closedir(d);
    return Status::OK();
}

Status FileSystem::Stat(const std::string& filename, FileStat* stats) {
    CHECK_NE(stats, nullptr);
    struct stat sbuf;
    if (stat(filename.data(), &sbuf))
        return IOError(errno, filename);
    stats->size = sbuf.st_size;
    stats->mtime_nsec = sbuf.st_mtime * 1e9;
    stats->is_directory = S_ISDIR(sbuf.st_mode);
    return Status::OK();
}

Status FileSystem::FileSize(const std::string& filename, size_t* size) {
    *size = 0;
    FileStat stats;
    RETURN_IF_ERROR(Stat(filename, &stats));
    *size = stats.size;
    return Status::OK();
}

Status FileSystem::DeleteFile(const std::string& filename) {
    if (unlink(filename.data()))
        return IOError(errno, filename);
    return Status::OK();
}

Status FileSystem::CreateDir(const std::string& path) {
    if (mkdir(path.data(), 0755) && errno != EEXIST) {
        return IOError(errno, path);
    }
    return Status::OK();
}

Status FileSystem::RecursivelyCreateDir(const std::string& path) {
    Status s = CreateDir(path);
    if (s.ok() || !Status::IsNotFound(s))
        return s;

    std::string parent;
    if (EndWith(path, "/")) {
        parent = Dirname(path.substr(0, path.size() - 1));
    } else {
        parent = Dirname(path);
    }
    if (parent.empty())
        return Status::Unknown(path);
    RETURN_IF_ERROR(RecursivelyCreateDir(parent));
    return CreateDir(path);
}

Status FileSystem::IsDirectory(const std::string& path) {
    FileStat st;
    RETURN_IF_ERROR(Stat(path, &st));
    if (!st.is_directory)
        return Status::FailedPrecondition(path + " Not a directory");
    return Status::OK();
}

Status FileSystem::DeleteDir(const std::string& path) {
    if (rmdir(path.data()))
        return IOError(errno, path);
    return Status::OK();
}

Status FileSystem::RecursivelyDeleteDir(const std::string& path) {
    Status s = DeleteDir(path);
    if (s.ok() || !Status::IsFailedPrecondition(s))
        return s;

    std::vector<std::string> entries;
    RETURN_IF_ERROR(ScanDir(path, &entries));
    for (size_t i = 0; i < entries.size(); ++i) {
        std::string child(path + "/" + entries[i]);
        FileStat stat;
        RETURN_IF_ERROR(Stat(child, &stat));
        if (stat.is_directory) {
            RETURN_IF_ERROR(RecursivelyDeleteDir(child));
        } else {
            RETURN_IF_ERROR(DeleteFile(child));
        }
    }
    return DeleteDir(path);
}

Status FileSystem::RenameFile(const std::string& src, const std::string& target) {
    if (rename(src.data(), target.data()))
        return IOError(errno, src);
    return Status::OK();
}

Status FileSystem::CopyFile(const std::string& src, const std::string& target) {
    FileStat st;
    RETURN_IF_ERROR(Stat(src, &st));

    int inf = open(src.data(), O_RDONLY);
    if (inf < 0)
        return IOError(errno, src);
#define _PERMISSION (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)
    int of = open(target.data(), O_CREAT | O_WRONLY | O_TRUNC, _PERMISSION);
    if (of < 0) {
        close(inf);
        return IOError(errno, target);
    }
    ssize_t written_size;
    off_t offset = 0;
    while (st.size > 0 && (written_size = sendfile(of, inf, &offset, st.size)) > 0) {
        offset += written_size;
        st.size -= written_size;
    }
    close(inf);
    close(of);
    if (written_size < 0)
        return IOError(errno, "sendfile failed");
    return Status::OK();
}

Status FileSystem::NewFileInputStream(const std::string& filename,
                                      std::unique_ptr<InputStream>* result) {
    int fd = open(filename.data(), O_RDONLY);
    if (fd < 0)
        return IOError(errno, filename);
    result->reset(new FileInputStream(filename, fd, true));
    return Status::OK();
}

Status FileSystem::NewFileOutputStream(const std::string& filename,
                                       std::unique_ptr<OutputStream>* result) {
    int fd = open(filename.data(), O_CREAT | O_WRONLY | O_TRUNC, _PERMISSION);
    if (fd < 0)
        return IOError(errno, filename);
    result->reset(new FileOutputStream(filename, fd, true));
    return Status::OK();
}

Status Spawn(const std::string& cmd, int* child_ret) {
    std::vector<std::string> args;
    std::string::size_type start = 0, stop = 0;
    while (true) {
        stop = cmd.find(' ', start);
        if (stop == std::string::npos) {
            if (start < cmd.size())
                args.emplace_back(cmd, start);
            break;
        }
        if (cmd[start] == '\"') {
            ++start;
            stop = cmd.find('\"', start);
            if (stop == std::string::npos)
                return Status::InvalidArgument(cmd);
            if (cmd[stop + 1] != ' ')
                return Status::InvalidArgument(cmd);
            args.emplace_back(cmd, start, stop - start);
            start = stop + 2;
        } else {
            args.emplace_back(cmd, start, stop - start);
            start = stop + 1;
        }
    }

    std::string path(args[0]);
    char* argv[args.size() + 1];
    memset(argv, 0, sizeof(argv));
    argv[0] = const_cast<char*>(FileSystem::Basename(args[0]).data());
    for (size_t i = 1; i < args.size(); ++i)
        argv[i] = const_cast<char*>(args[i].data());

    pid_t pid = fork();
    if (pid < 0)
        return IOError(errno, "fork failed");
    if (pid == 0) {
        // On children process.
        write(STDIN_FILENO, "passwd", 6);
        if (execvp(path.data(), argv) < 0)
            return IOError(errno, "execvp failed");
    }
    int status = 0;
    pid_t ret = -1;
    do {
        ret = waitpid(pid, &status, 0);
    } while (ret < 0 && errno == EINTR);
    if (ret < 0)
        return IOError(errno, "waitpid failed");
    CHECK_EQ(ret, pid);
    if (child_ret != nullptr)
        *child_ret = WEXITSTATUS(status);
    return Status::OK();
}

class Crypto::CryptoImpl {
public:
    CryptoImpl(const std::string& key);
    ~CryptoImpl() = default;

    Status Encrypt(const InputStream* in, OutputStream* out);
    Status Decrypt(const InputStream* in, OutputStream* out);

private:
    crypt_data data;
    DISALLOW_COPY_AND_ASSIGN(CryptoImpl);
};

#define UP_ALIGNED(size, aligment) ((size + aligment - 1) & ~(aligment - 1))

static inline void ArrayToBlock(const char array[8], char block[64]) {
    for (auto i = 0; i < 8; ++i)
        for (auto j = 0; j < 8; ++j)
            block[i * 8 + j] = (array[i] >> j) & 1;
}

static inline void BlockToArray(const char block[64], char array[8]) {
    memset(array, 0, 8);
    for (auto i = 0; i < 8; ++i)
        for (auto j = 0; j < 8; ++j)
            array[i] |= block[i * 8 + j] << j;
}

Crypto::CryptoImpl::CryptoImpl(const std::string& key) {
    char array[8] = {0};
    key.copy(array, key.size() < 8 ? key.size() : 8);
    char block[64];
    ArrayToBlock(array, block);
    memset(&data, 0, sizeof(data));
    setkey_r(block, &data);
}

Status Crypto::CryptoImpl::Encrypt(const InputStream* in, OutputStream* out) {
    uint32_t in_size = 0;
    std::string buffer;
    std::string array;
    char block[64];
    Status s;
    while ((s = in->Read(8, &array)).ok()) {
        in_size += array.size();
        if (array.size() < 8) {
            char new_array[8] = {0};
            std::copy(array.begin(), array.end(), new_array);
            array = std::string(new_array, 8);
        }
        ArrayToBlock(array.data(), block);
        encrypt_r(block, 0, &data);
        BlockToArray(block, const_cast<char*>(array.data()));
        buffer.append(array.data(), 8);
    }
    if (!Status::IsOutOfRange(s))
        return s;
    s.Clear();
    in_size = htonl(in_size);
    std::string head(reinterpret_cast<char*>(&in_size), sizeof(in_size));
    RETURN_IF_ERROR(out->Append(head));
    RETURN_IF_ERROR(out->Append(buffer));
    return s;
}

Status Crypto::CryptoImpl::Decrypt(const InputStream* in, OutputStream* out) {
    uint32_t in_size = 0;
    std::string buffer;
    std::string array;
    char block[64];
    Status s;
    s = in->Read(sizeof(in_size), &array);
    if (!s.ok() || array.size() != sizeof(in_size))
        return Status::DataLoss("Input parameter bytes to less in decrypt.");
    in_size = ntohl(*reinterpret_cast<const uint32_t*>(array.data()));
    while ((s = in->Read(8, &array)).ok()) {
        if (array.size() != 8)
            return Status::DataLoss("Input stream is incremented by the number of bytes read in decrypt.");
        ArrayToBlock(array.data(), block);
        encrypt_r(block, 1, &data);
        BlockToArray(block, const_cast<char*>(array.data()));
        buffer.append(array);
    }
    if (!Status::IsOutOfRange(s))
        return s;
    s.Clear();
    RETURN_IF_ERROR(out->Append(buffer.substr(0, in_size)));
    return s;
}

Crypto::Crypto(const std::string& key): key_(key), impl_(new CryptoImpl(key)) {}

Crypto::~Crypto() = default;

Status Crypto::Encrypt(const InputStream* in, OutputStream* out) {
    return impl_->Encrypt(in, out);
}

Status Crypto::Decrypt(const InputStream* in, OutputStream* out) {
    return impl_->Decrypt(in, out);
}

// The zlib format was designed to be compact and fast for use in memory
// and on communications channels.
// The gzip format was designed for single-file compression on file systems,
// has a larger header than zlib to maintain directory information, and uses
// a different, slower check method than zlib.
struct ZlibOptions {
    enum FlushMode {
        FLUSH_MODE_MIN = Z_NO_FLUSH,
        NO_FLUSH = Z_NO_FLUSH,
        PARTIAL_FLUSH = Z_PARTIAL_FLUSH,
        SYNC_FLUSH = Z_SYNC_FLUSH,
        FULL_FLUSH = Z_FULL_FLUSH,
        FINISH = Z_FINISH,
        BLOCK = Z_BLOCK,
        TREES = Z_TREES,
        FLUSH_MODE_MAX = Z_TREES,
    };

    enum CompressionStrategy {
        COMPRESSION_STRATEGY_MIN = Z_DEFAULT_STRATEGY,
        DEFAULT_STRATEGY = Z_DEFAULT_STRATEGY,
        FILTERED = Z_FILTERED,
        HUFFMAN_ONLY = Z_HUFFMAN_ONLY,
        RLE = Z_RLE,
        FIXED = Z_FIXED,
        COMPRESSION_STRATEGY_MAX = Z_FIXED,
    };

    static const int kCompressionLevelMin = Z_DEFAULT_COMPRESSION;
    static const int kCompressionLevelMax = Z_BEST_COMPRESSION;
    static const int kCompressionLevelDefault = Z_DEFAULT_COMPRESSION;
    static const int kCompressionLevelNoCompression = Z_NO_COMPRESSION;
    static const int kCompressionLevelBestSpeed = Z_BEST_SPEED;
    static const int kCompressionLevelBestCompression = Z_BEST_COMPRESSION;

    static const int kMemoryLevelMin = 1;
    static const int kMemoryLevelMax = 9;
    static const int kMemoryLevelDefault = 8;

    static const int kCompressionMethodMin = Z_DEFLATED;
    static const int kCompressionMethodMax = Z_DEFLATED;
    static const int kCompressionMethodDefault = Z_DEFLATED;

    static const int kMaxWindowBits = MAX_WBITS;

    size_t input_buffer_size;
    size_t output_buffer_size;

    int flush_mode;
    int window_bits;

    // From the zlib manual (http://www.zlib.net/manual.html):
    // The compression level must be kCompressionLevelDefault, or
    // between kCompressionLevelMin and kCompressionLevelMax:
    // kCompressionLevelBestSpeed: gives best speed.
    // kCompressionLevelBestCompression: gives best compression.
    // kCompressionLevelNoCompression: gives no compression at all,
    // the input data is simply copied a block at a time.
    // kCompressionLevelDefault: gives a default compromise between speed and
    // compression (currently equivalent to level 6).
    int compression_level;

    // The only kCompressionMethodDefault supported at this time.
    int compression_method;

    // From the zlib manual (http://www.zlib.net/manual.html):
    // The mem_level parameter specifies how much memory should be allocated
    // for the internal compression state.
    // kMemoryLevelMin: uses minimum memory but is slow and reduces compression ratio.
    // kMemoryLevelMax: uses maximum memory for optimal speed.
    // The default value is kMemoryLevelDefault(8).
    int mem_level;

    // From the zlib manual (http://www.zlib.net/manual.html):
    // The strategy parameter is used to tune the compression algorithm. Use the
    // value DEFAULT_STRATEGY for normal data, FILTERED for data produced by
    // a filter (or predictor), HUFFMAN_ONLY to force Huffman encoding only
    // (no string match), or RLE to limit match distances to one (run-length encoding).
    // Filtered data consists mostly of small values with a somewhat random distribution.
    // In this case, the compression algorithm is tuned to compress them better.
    // The effect of FILTERED is to force more Huffman coding and less string matching;
    // it is somewhat intermediate between DEFAULT_STRATEGY and HUFFMAN_ONLY.
    // RLE is designed to be almost as fast as HUFFMAN_ONLY, but give better compression for
    // PNG image data. The strategy parameter only affects the compression ratio
    // but not the correctness of the compressed output even if it is not set
    // appropriately. FIXED prevents the use of dynamic Huffman codes, allowing
    // for a simpler decoder for special applications.
    int compression_strategy;

    ZlibOptions(long input_buffer_size, long output_buffer_size, int flush_mode, int  window_bits,
                int compression_level, int  compression_method, int  mem_level, int  compression_strategy)
        : input_buffer_size(input_buffer_size), output_buffer_size(output_buffer_size),
          flush_mode(flush_mode), window_bits(window_bits),
          compression_level(compression_level), compression_method(compression_method),
          mem_level(mem_level), compression_strategy(compression_strategy) {
#define CHECK_IN_RANGE(value, min, max) (((value) >= (min)) || ((value) <= (max)))
        CHECK_IN_RANGE(flush_mode, FlushMode::FLUSH_MODE_MIN, FlushMode::FLUSH_MODE_MAX);
        CHECK_IN_RANGE(compression_level, kCompressionLevelMin, kCompressionLevelMax);
        CHECK_IN_RANGE(compression_method, kCompressionMethodMin, kCompressionMethodMax);
        CHECK_IN_RANGE(mem_level, kMemoryLevelMin, kMemoryLevelMax);
        CHECK_IN_RANGE(compression_strategy,
                       CompressionStrategy::COMPRESSION_STRATEGY_MIN,
                       CompressionStrategy::COMPRESSION_STRATEGY_MAX);
    }

    ~ZlibOptions() = default;

    static ZlibOptions Default() {
        return ZlibOptions(256 << 10, 256 << 10, FlushMode::NO_FLUSH, kMaxWindowBits,
                           kCompressionLevelDefault, kCompressionMethodDefault,
                           kMemoryLevelDefault, CompressionStrategy::DEFAULT_STRATEGY);
    }
};

class ZlibDeflate::ZlibDeflateImpl {
public:
    ZlibDeflateImpl(const ZlibOptions& opts = ZlibOptions::Default());
    ~ZlibDeflateImpl() = default;

    Status operator()(const InputStream* in, OutputStream* out);

private:
    ZlibOptions opts_;
    DISALLOW_COPY_AND_ASSIGN(ZlibDeflateImpl);
};

ZlibDeflate::ZlibDeflateImpl::ZlibDeflateImpl(const ZlibOptions& opts): opts_(opts) {}

Status ZlibDeflate::ZlibDeflateImpl::operator()(const InputStream* in, OutputStream* out) {
    std::string input_buffer;
    std::string output_buffer(opts_.output_buffer_size, 0);
    std::string result;
    z_stream zstream;
    memset(&zstream, 0, sizeof(zstream));
    zstream.zalloc = Z_NULL;
    zstream.zfree = Z_NULL;
    zstream.opaque = Z_NULL;
    int ret = deflateInit2(&zstream, opts_.compression_level, opts_.compression_method,
            opts_.window_bits, opts_.mem_level, opts_.compression_strategy);
    if (ret != Z_OK)
        return Status::InvalidArgument(zstream.msg);
    int flush = false;
    Status s;

    do {
        input_buffer.clear();
        s = in->Read(opts_.input_buffer_size, &input_buffer);
        if (!s.ok() && !s.IsOutOfRange()) {
            deflateEnd(&zstream);
            return s;
        }
        flush = s.IsOutOfRange();
        zstream.next_in = reinterpret_cast<unsigned char*>(const_cast<char*>(input_buffer.data()));
        zstream.avail_in = input_buffer.size();

        do {
            zstream.next_out = reinterpret_cast<unsigned char*>(const_cast<char*>(output_buffer.data()));
            zstream.avail_out = output_buffer.size();
            ret = deflate(&zstream, flush ? utils::ZlibOptions::FINISH : opts_.flush_mode);
            CHECK_NE(ret, Z_STREAM_ERROR);
            size_t have = output_buffer.size() - zstream.avail_out;
            result.append(output_buffer.begin(), output_buffer.begin() + have);
        } while (zstream.avail_out == 0);
        CHECK_EQ(zstream.avail_in, 0);
    } while (!flush);
    s.Clear();
    deflateEnd(&zstream);
    out->Append(result);
    return s;
}

ZlibDeflate::ZlibDeflate(): impl_(new ZlibDeflateImpl) {}

ZlibDeflate::~ZlibDeflate() = default;

Status ZlibDeflate::operator ()(const InputStream* in, OutputStream* out) {
    return impl_->operator()(in, out);
}

class ZlibInflate::ZlibInflateImpl {
public:
    ZlibInflateImpl(const ZlibOptions& opts = ZlibOptions::Default());
    ~ZlibInflateImpl() = default;

    Status operator()(const InputStream* in, OutputStream* out);

private:
    ZlibOptions opts_;
    DISALLOW_COPY_AND_ASSIGN(ZlibInflateImpl);
};

ZlibInflate::ZlibInflateImpl::ZlibInflateImpl(const ZlibOptions& opts): opts_(opts) {}

Status ZlibInflate::ZlibInflateImpl::operator ()(const InputStream* in, OutputStream* out) {
    std::string input_buffer;
    std::string output_buffer(opts_.output_buffer_size, 0);
    std::string result;
    z_stream zstream;
    memset(&zstream, 0, sizeof(zstream));
    zstream.zalloc = Z_NULL;
    zstream.zfree = Z_NULL;
    zstream.opaque = Z_NULL;
    int ret = inflateInit2(&zstream, opts_.window_bits);
    if (ret != Z_OK)
        return Status::InvalidArgument(zstream.msg);

    Status s;
    do {
        input_buffer.clear();
        s = in->Read(opts_.input_buffer_size, &input_buffer);
        if (!s.ok() && !s.IsOutOfRange()) {
            inflateEnd(&zstream);
            return s;
        }
        if (s.IsOutOfRange())
            break;
        zstream.next_in = reinterpret_cast<unsigned char*>(const_cast<char*>(input_buffer.data()));
        zstream.avail_in = input_buffer.size();
        do {
            zstream.next_out = reinterpret_cast<unsigned char*>(const_cast<char*>(output_buffer.data()));
            zstream.avail_out = output_buffer.size();
            ret = inflate(&zstream, opts_.flush_mode);
            CHECK_NE(ret, Z_STREAM_ERROR);
            if (ret == Z_NEED_DICT)
                ret = Z_DATA_ERROR;
            if (ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
                inflateEnd(&zstream);
                return Status::InvalidArgument(zstream.msg);
            }
            size_t have = output_buffer.size() - zstream.avail_out;
            result.append(output_buffer.begin(), output_buffer.begin() + have);
        } while (zstream.avail_out == 0);
    } while (ret != Z_STREAM_END);
    inflateEnd(&zstream);
    s.Clear();
    out->Append(result);
    return s;
}

ZlibInflate::ZlibInflate(): impl_(new ZlibInflateImpl) {}

ZlibInflate::~ZlibInflate() = default;

Status ZlibInflate::operator ()(const InputStream* in, OutputStream* out) {
    return impl_->operator()(in, out);
}

namespace internal {
bool Strtoll(const std::string& str, long long int* result, int base) {
    errno = 0;
    long long int value = strtoll(str.data(), NULL, base);
    if (errno)
        return false;
    *result = value;
    return true;
}
} // namespace internal

Date Today() {
    struct timeval tv;
    memset(&tv, 0, sizeof(tv));
    gettimeofday(&tv, NULL);
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    localtime_r(&tv.tv_sec, &tm);
    Date d;
    d.year = tm.tm_year + 1900;
    d.month = tm.tm_mon + 1;
    d.day = tm.tm_mday;
    d.hours = tm.tm_hour;
    d.minutes = tm.tm_min;
    d.seconds = tm.tm_sec;
    d.micro_seconds = tv.tv_usec;
    d.weekday = tm.tm_wday == 0 ? 7 : tm.tm_wday;
    return d;
}

class License::LicenseImpl {
public:
    LicenseImpl(const std::string& cipher);
    ~LicenseImpl() = default;

    static Status Generator(const std::string& identify, uint64_t magic,
                            const std::string& expired, std::string* cipher);

    inline const std::string& identify() const;
    inline uint64_t magic() const;
    const std::string Date() const;

    bool IsValid() const;
    bool IsExpired() const;

private:
    std::string cipher_;
    int year_;
    int mon_;
    int day_;
    std::string identify_;
    uint64_t magic_;
    static const size_t kIDSize = 16;
    static const size_t kMaxBufferSize = 200;
    static const std::string pass_;
};

const std::string License::LicenseImpl::pass_("FiberHome CopyRight@");

uint16_t Checksum(const uint16_t *buf, size_t size)
{
    uint64_t chksum = 0;
    for (size_t i = 0; i < size; i++)
        chksum += buf[i];
    chksum = (chksum & 0xFFFF) + (chksum >> 16);
    chksum += (chksum >> 16);
    return ~chksum;
}

License::LicenseImpl::LicenseImpl(const std::string& cipher):
        cipher_(), year_(0), mon_(0), day_(0), identify_(), magic_(0) {
    char c;
    cipher_.resize(cipher.size() / 2, 0);
    for (size_t i = 0; i < cipher.size() / 2; ++i) {
        if (!StrtoInt(cipher.substr(i * 2, 2), &c, 16))
            return;
        cipher_[i] = c & 0xFF;
    }

    Crypto crypto(pass_);
    std::string buffer;
    StringInputStream in(cipher_);
    StringOutputStream out(&buffer);
    crypto.Decrypt(&in, &out);

    // Reverse checksum to ensure the consistency
    size_t n = buffer.size() + (buffer.size() % 2);
    uint16_t checksum = Checksum(reinterpret_cast<const uint16_t*>(buffer.data()), n / 2);
    if (checksum != 0) {
        return;
    }
    int year = 0, mon = 0, day = 0;
    uint64_t magic = 0;
    char id[kIDSize + 1] = {0};
    int counts = sscanf(buffer.data() + 2, "%d %d %d %lu %s", &year, &mon, &day, &magic, id);
    if (counts == 5) {
        year_ = year;
        mon_ = mon;
        day_ = day;
        identify_ = id;
        magic_ = magic;
    }
}

Status License::LicenseImpl::Generator(const std::string& identify, uint64_t magic,
                                       const std::string& expired, std::string* cipher) {
    int year = 0, mon = 0, day = 0;

    std::string::size_type start = 0, end;
    end = expired.find('/', start);
    if (end == std::string::npos)
        return Status::InvalidArgument("expired form error");
    if (!StrtoInt(expired.substr(start, end - start), &year))
        return Status::InvalidArgument("expired value error");
    start = end + 1;
    end = expired.find('/', start);
    if (end == std::string::npos)
        return Status::InvalidArgument("expired form error");
    if (!StrtoInt(expired.substr(start, end - start), &mon))
        return Status::InvalidArgument("expired value error");
    start = end + 1;
    if (!StrtoInt(expired.substr(start), &day))
        return Status::InvalidArgument("expired value error");

    std::string id = identify.size() > kIDSize ? identify.substr(0, kIDSize) : identify;

    char buffer[kMaxBufferSize] = {0};
    int n = snprintf(buffer + 2, kMaxBufferSize - 2, "%d %d %d %lu %s", year, mon, day, magic, id.data());
    n += (n % 2);
    uint16_t* checksum = reinterpret_cast<uint16_t*>(buffer);
    *checksum = 0;
    *checksum = Checksum(reinterpret_cast<uint16_t*>(buffer), (n + 2) / 2);

    Crypto crypto(pass_);
    std::string result;
    StringInputStream in(buffer, n + 2);
    StringOutputStream out(&result);
    Status s = crypto.Encrypt(&in, &out);
    if (s.ok()) {
        cipher->clear();
        char tmp[5];
        for (size_t i = 0; i < result.size(); ++i) {
            snprintf(tmp, 5, "%02X", result[i] & 0xFF);
            cipher->append(tmp);
        }
    }
    return s;
}

inline const std::string& License::LicenseImpl::identify() const { return identify_; }

inline uint64_t License::LicenseImpl::magic() const { return magic_; }

const std::string License::LicenseImpl::Date() const {
    char buffer[50] = {0};
    snprintf(buffer, 50, "%d/%d/%d", year_, mon_, day_);
    return buffer;
}

bool License::LicenseImpl::IsValid() const {
    return (!cipher_.empty()) && year_ > 0 && mon_ > 0 && day_ > 0;
}

bool License::LicenseImpl::IsExpired() const {
    if (!IsValid())
        return true;
    utils::Date d = Today();
#define CHECK_EQ_TODO(a, b, Doing)  \
    if ((a) > (b)) {                \
        return true;                \
    } else if ((a) < (b)) {         \
        return false;               \
    } else {                        \
        Doing;                      \
    }
    CHECK_EQ_TODO(d.year, year_, CHECK_EQ_TODO(d.month, mon_, CHECK_EQ_TODO(d.day, day_, return false)));
    // Below expressions do nothing and used to remove "No return, in
    // function returning non-void" failed on the IDE, it never be invoked.
    return false;
}

License::License(const std::string& cipher): impl_(new LicenseImpl(cipher)) {}

License::~License() = default;

Status License::Generator(const std::string& indentify, uint64_t magic,
                          const std::string& expired, std::string* cipher) {
    return LicenseImpl::Generator(indentify, magic, expired, cipher);
}

const std::string& License::Identify() const { return impl_->identify(); }

uint64_t License::Magic() const { return impl_->magic(); }

const std::string License::Date() const { return impl_->Date(); }

bool License::IsValid() const { return impl_->IsValid(); }

bool License::IsExpired() const { return impl_->IsExpired(); }
} // namespace utils

#ifdef UTILS_TEST
using namespace utils;

#define UNIT_TEST(enable, XXX) while (enable) { \
    XXX \
    break; \
}

int main() {
    // Status unit test
#define XXX(CONST, FUNC, STR) CHECK(Status::Is##FUNC(Status(error::CONST, STR)));
    UNIT_TEST(true,
        MAKE_ERROR_CODE(XXX)
    );
#undef XXX

    // FileSystem unit test
    UNIT_TEST(true,
        CHECK_EQ(FileSystem::IsAbsolutePath("/xxx"), true);
        CHECK_EQ(FileSystem::IsAbsolutePath("./xxx"), false);
        CHECK_EQ(FileSystem::Dirname("/"), "/");
        CHECK_EQ(FileSystem::Dirname("xxx").empty(), true);
        CHECK_EQ(FileSystem::Dirname("/path/name"), "/path");
        CHECK_EQ(FileSystem::Dirname("../path/name"), "../path");
        CHECK_EQ(FileSystem::Basename("/").empty(), true);
        CHECK_EQ(FileSystem::Basename("xxx"), "xxx");
        CHECK_EQ(FileSystem::Basename("/path/name"), "name");
        CHECK_EQ(FileSystem::Basename("../path/name"), "name");
        CHECK_EQ(FileSystem::Extension("/path/name").empty(), true);
        CHECK_EQ(FileSystem::Extension("../path/name.ext"), "ext");
        CHECK(FileSystem::FileExists("/dev/null").ok());
        CHECK(FileSystem::FileExists("/path/dummy.file").IsNotFound());
        FileStat st;
        CHECK(FileSystem::Stat("/dev/null", &st).ok());
        CHECK_EQ(st.is_directory, false);
        CHECK(FileSystem::CreateDir("./dir").ok());
        CHECK(FileSystem::IsDirectory("./dir").ok());
        CHECK(FileSystem::DeleteDir("./dir").ok());
        CHECK(FileSystem::RecursivelyCreateDir("./dir/dir1").ok());
        CHECK(FileSystem::IsDirectory("./dir/dir1").ok());
        CHECK(FileSystem::RecursivelyDeleteDir("./dir").ok());
    );

    // InputStream, OutputStream unit test
    std::unique_ptr<InputStream> in;
    std::unique_ptr<OutputStream> out;

    // FileInputStream, FileOutputStream unit test
    UNIT_TEST(true,
        std::string content("test");
        std::string file("file");
        CHECK(FileSystem::NewFileOutputStream(file, &out).ok());
        CHECK(out->Append(content).ok());
        CHECK(FileSystem::NewFileInputStream(file, &in).ok());
        std::string result;
        CHECK(in->Read(1000, &result).ok());
        CHECK_EQ(result, content);

        result.clear();
        std::string from("test");
        std::string to;
        in.reset(new StringInputStream(from));
        CHECK(in->Read(1000, &result).ok());
        CHECK_EQ(from, result);
        out.reset(new StringOutputStream(&to));
        CHECK(out->Append(result).ok());
        CHECK_EQ(to, result);
        FileSystem::DeleteFile(file);
    );

    // Encrypt/Decrypt unit test
    UNIT_TEST(true,
        std::string key("1234567");
        std::string content("This is an examples that show how to encrypt/decrypt files and strings");
        std::string sin;
        std::string sout;
        std::string fin("fin");
        std::string fout("fout");

        CHECK(EncryptString(key, content, &sin).ok());
        CHECK(DecryptString(key, sin, &sout).ok());
        CHECK_EQ(sout, content);

        CHECK(EncryptStringToFile(key, content, fin).ok());
        CHECK(DecryptFile(key, fin, fout).ok());
        FileStat st;
        CHECK(FileSystem::Stat(fout, &st).ok());
        CHECK_EQ(static_cast<size_t>(st.size), content.size());

        sin.clear();
        CHECK(EncryptFileToString(key, fout, &sin).ok());
        sout.clear();
        CHECK(DecryptString(key, sin, &sout).ok());
        CHECK_EQ(sout, content);

        FileSystem::DeleteFile(fin);
        FileSystem::DeleteFile(fout);
    );

    // Deflate/Inflate unit test
    UNIT_TEST(true,
        std::string content("This is an examples that show how to encrypt/decrypt files and strings");
        std::string sin;
        std::string sout;
        std::string fin("fin");
        std::string fout("fout");

        CHECK(DeflateString(content, &sin).ok());
        CHECK(InflateString(sin, &sout).ok());
        CHECK_EQ(sout, content);

        CHECK(DeflateStringToFile(content, fin).ok());
        CHECK(InflateFile(fin, fout).ok());
        FileStat st;
        CHECK(FileSystem::Stat(fout, &st).ok());
        CHECK_EQ(static_cast<size_t>(st.size), content.size());

        sin.clear();
        CHECK(DeflateFileToString(fout, &sin).ok());
        sout.clear();
        CHECK(InflateString(sin, &sout).ok());
        CHECK_EQ(sout, content);

        FileSystem::DeleteFile(fin);
        FileSystem::DeleteFile(fout);
    );

    // License unit test
    UNIT_TEST(true,
        std::string cipher;
        std::string id("AABBCCDDEEFF");
        std::string expired("2000/5/2");
        uint64_t magic = 100100;
        CHECK(License::Generator(id, magic, expired, &cipher).ok());

        License license0(cipher);
        CHECK(license0.IsValid());
        CHECK_EQ(license0.Identify(), id);
        CHECK_EQ(license0.Magic(), magic);
        CHECK(license0.IsExpired() == true);

        cipher.clear();
        expired = "2020/5/3";
        CHECK(License::Generator(id, magic, expired, &cipher).ok());
        License license1(cipher);
        CHECK(license1.IsValid());
        CHECK_EQ(license1.Identify(), id);
        CHECK_EQ(license1.Magic(), magic);
        CHECK(license1.IsExpired() == false);

        License license2("0000001E9FB7AE9C914F0E08BD8657BD95B027A177FA02C972EB2B9B8FC4A6D0EAC4B7BF");
        CHECK(license2.IsValid());
        CHECK_EQ(license2.Identify(), id);
        CHECK_EQ(license2.Magic(), magic);
        CHECK(license2.IsExpired() == false);
    );

    return 0;
}
#endif




