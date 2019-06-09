#include <unistd.h>
#include <iostream>
#include <utils.h>

int main(void) {
    auto res = utils::Spawn("ssh root@172.16.23.4");
    std::cout << "utils::Spawn " << res.ToString() << std::endl;

    return 0;
}
