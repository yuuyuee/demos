#include <iostream>
#include <stdint.h>
#include <string.h>

#define CPUID(a, b, c, d, in) \
    __asm__("mov %%rbx, %%rdi\n"                    \
        "cpuid\n"                                   \
        "xchg %%rdi, %%rbx\n"                       \
        : "=a" (a), "=D" (b), "=c" (c), "=d" (d)    \
        : "a" (in))

int main(void) {
    uint32_t eax, ebx, ecx, edx;
    CPUID(eax, ebx, ecx, edx, 0);
    char vendor[13] = {0};
    memcpy(vendor, &ebx, 4);
    memcpy(vendor + 4, &edx, 4);
    memcpy(vendor + 8, &ecx, 4);
    CPUID(eax, ebx, ecx, edx, 1);
    int family = (eax >> 8) & 0x0f;
    int model = (eax >> 4) & 0x0f;
    if (family == 0x0f) {
        family += (eax >> 20) & 0xFF;
        model += ((eax >> 16) & 0x0f) << 4;
    }

    std::cout << "family: " << family << std::endl;
    std::cout << "model: " << model << std::endl;
    std::cout << "vendor: " << vendor << std::endl;
    return 0;
}
