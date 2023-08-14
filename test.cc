#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unordered_map>
#include <map>

int main() {
    if (__builtin_types_compatible_p(uint8_t, uint8_t))
        printf("_builtin_types_compatible_p\n");

    printf("%s\n", strerror(-1));
    return 0;
}
