#include <iostream>
#include <cstring>
#include <cstdio>
#include <chrono>

int main() {
    time_t now = time(nullptr);
    struct tm ltm;
    localtime_r(&now, &ltm);
    char buffer[20] = {0};
    strftime(buffer, 20, "%Y%m%d%H%M%S", &ltm);
    memset(&ltm, 0, sizeof(ltm));
    strptime(buffer, "%Y%m%d%H%M%S", &ltm);
    std::cout << mktime(&ltm) << ": " << now << std::endl;
    return 0;
}
