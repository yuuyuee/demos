// Author: X3130 (yuyue) X3130@njsecnet.com

#include <iostream>
#include <unistd.h>
#include <string>
#include <chrono>
#include <system_error>
#include <mutex>
#include <csignal>
#include <curl/curl.h>


void MakeSystemError() {
    throw std::system_error(errno, std::generic_category());
}

int UnixReturnCheck(int ret) {
    if (ret < 0)
        MakeSystemError();
    return ret;
}

void curl_global_init_wrapper() {
    auto ret = curl_global_init(CURL_GLOBAL_ALL);
    if (ret != CURLE_OK) {
        throw std::runtime_error(curl_easy_strerror(ret));
    }
}

void AlarmIgnore() {
    auto ret = signal(SIGALRM, SIG_IGN);
    if (ret == SIG_ERR) MakeSystemError();
}

void AlarmDefault() {
    auto ret = signal(SIGALRM, SIG_DFL);
    if (ret == SIG_ERR) MakeSystemError();
}

size_t AppendToStream(char *ptr, size_t size, size_t nitems, void *ostream) {
    std::string* buffer = reinterpret_cast<std::string*>(ostream);
    size_t total = size * nitems;
    buffer->append(ptr, total);
    return total;
}

bool wget(const std::string& path, const std::string& local, int timeout) {
    static std::once_flag flag;
    std::call_once(flag, curl_global_init_wrapper);
#define CURLCODE_CHECK(CURLCODE, func)                              \
    if (CURLCODE != CURLE_OK) {                                     \
        curl_easy_cleanup(curl);                                    \
        std::cout << #func << "faild (" <<  CURLCODE <<  "): "      \
                    << curl_easy_strerror(CURLCODE) << std::endl;   \
        return false;                                               \
    }
    CURL *curl = curl_easy_init();
    if (!curl) {
        errno = ENOMEM;
        UnixReturnCheck(-1);
    }
    CURLCODE_CHECK(curl_easy_setopt(curl, CURLOPT_URL, path.data()), CURLOPT_URL);
    CURLCODE_CHECK(curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1L), CURLOPT_FORBID_REUSE);
    //CURLCODE_CHECK(curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L), CURLOPT_NOSIGNAL);
    CURLCODE_CHECK(curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout), CURLOPT_TIMEOUT);
    CURLCODE_CHECK(curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout), CURLOPT_CONNECTTIMEOUT);
    CURLCODE_CHECK(curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L), CURLOPT_AUTOREFERER);
    CURLCODE_CHECK(curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L), CURLOPT_FOLLOWLOCATION);
    CURLCODE_CHECK(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, AppendToStream), CURLOPT_WRITEFUNCTION);
    std::string buffer;
    CURLCODE_CHECK(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer), CURLOPT_WRITEDATA);
    AlarmIgnore();
    printf("Download %s\n", path.data());
    CURLCODE_CHECK(curl_easy_perform(curl), curl_easy_perform);
    AlarmDefault();
    curl_easy_cleanup(curl);
    if (buffer.empty()) return false;
    std::cout << buffer << std::endl;
    return true;
}

int main() {
    wget("http://172.16.6.134:1980/fs/fhfs/v1/dmwR/20180913/deft/X8Jgu2/3/333413/3010/62629d10-b74b-11e8-c000-000c2995db4d-139808671135488-8664.http",
         "curl.http", 5);
    return 0;
}

