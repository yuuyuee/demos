#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdint>
#include <string>

#define PATH "/home/yuyue/"
#define _DIR "workdir"
#define DIRECTORY (PATH _DIR)

inline uint64_t MACToInt(const char mac[12]) {
    uint64_t result = 0;
    for (auto i = 0; i < 12; ++i) {
        uint64_t tmp = mac[i];
        if (tmp >= 'A' && tmp <= 'F') {
            result |= ((tmp - 'A' + 10) << ((12 - i - 1) * 4));
        } else if (tmp >= 'a' && tmp <= 'f') {
            result |= ((tmp - 'A' + 10) << ((12 - i - 1) * 4));
        } else if (tmp >= '0' && tmp <= '9') {
            result |= ((tmp - '0') << ((12 - i - 1) * 4));
        }
    }
    return result;
}

void Print(const std::string& arg = DIRECTORY) {
    std::cout << arg << std::endl;
}

inline void IntToMAC(const uint64_t v, char mac[12]) {
    const char table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    for (auto i = 0; i < 12; ++i)
        mac[i] = table[(v >> ((12 - i - 1) * 4)) & 0x0F];
}

int main(int argc, char* argv[]) {
#if 1
    for (int i = 1; i < 100; ++i) {
        char mac[12];
        IntToMAC(i, mac);
        printf("%12s\t"
                "138%08d\t"
                "%08d\t"
                "%08d\t"
                "\t"
                "\t"
                "%04d\n",
                std::string(mac, 12).data(),
                i * 138,
                i * 4,
                i * 8,
                i);
    }
#else
    uint64_t v = 0x121314;
    std::cout << std::setbase(16)
              << std::noshowbase
              << std::setiosflags(std::ios::uppercase)
              << std::setfill('0')
              << std::setw(12)
              << v
              << std::endl;

    char mac[12] = {0};
    IntToMAC(v, mac);
    std::cout << std::string(mac, 12) << std::endl;

    std::cout << std::setbase(16) << MACToInt(mac) << std::endl;
#endif
    Print();
    return 0;
}
