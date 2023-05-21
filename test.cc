#include <stdint.h>
#include <stdio.h>

int main() {
    uint32_t a = 0x00010001;
    uint8_t b = a;
    printf("%d\n", b);
    return 0;
}
