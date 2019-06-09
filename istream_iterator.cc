#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << argv[0] << " file" << std::endl;
        return 0;
    }
    std::vector<char> buffer;
    std::string file(argv[1]);
    std::ifstream in(file, std::ios::binary);
    if (!in) return -1;
    std::copy(std::istream_iterator<char>(in),
            std::istream_iterator<char>(),
            back_inserter(buffer));
    std::cout << buffer.size() << std::endl;
    return 0;
}
