#include <iostream>
#include <functional>

int main() {
    std::string name("hello world");
    std::hash<std::string> hasher;
    size_t hash = hasher(name);
    std::cout << "string: " << name << ", hash: " << hash << ", size = " << sizeof(hash) << std::endl;

    float i = 10.01;
    std::hash<float> _hasher;
    hash = _hasher(i);
    std::cout << "value: " << i << ", hash: " << hash << ", size = " << sizeof(hash) << std::endl;
    std::cout << std::hash<int>()(*(int*)&i) << std::endl;

    std::cout << "Size: " << sizeof(std::hash<int>{}(1)) << std::endl;

    std::cout << "key 1, hash " << std::hash<int>{}(1) << std::endl;
    return 0;
}
