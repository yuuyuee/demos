#include <iostream>
#include <fstream>
#include <cassert>
#include <cstddef>

inline uint64_t MACToInt(const char mac[12]) {
    uint64_t result = 0;
    for (auto i = 0; i < 12; ++i) {
        uint64_t tmp = mac[i];
        if ((tmp >= 'A' && tmp <= 'F') || (tmp >= 'a' && tmp <= 'f')) {
            result |= ((tmp - 'A' + 10) << (i * 4));
        } else if (tmp >= '0' && tmp <= '9') {
            result |= ((tmp - '0') << (i * 4));
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
    std::string in;
    std::string out;
    if (argc != 3) {
        std::cout << "macgen in out" << std::endl;
        return -1;
    }
    in = argv[1];
    out = argv[2];

    std::ifstream inf(in);
    std::ofstream outf(out);
    std::string block(sizeof(uint64_t) + sizeof(uint8_t), 0);
    uint64_t* mac = reinterpret_cast<uint64_t*>(const_cast<char*>(block.data()));
    uint8_t* attr = reinterpret_cast<uint8_t*>(const_cast<char*>(block.data() + sizeof(uint64_t)));
    *attr = 1u;
    std::string buffer;

    while (true) {
        buffer.clear();
        std::getline(inf, buffer);
        if (buffer.size() >= 12) {
            buffer.resize(buffer.size() - 1);
            *mac = MACToInt(buffer.data());
            outf.write(block.data(), block.size());
        } else {
            break;
        }
    }

    return 0;
}
