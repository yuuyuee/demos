#include <cstdio>
#include <cstring>
#include <unistd.h>


void Progress(const char* title, int rate) {
    char sign[100];
    for (int i = 0; i < 100; ++i)
        sign[i] = i < rate ? '#' : ' ';
    char buffer[1024];
    int n = snprintf(buffer, 1023, "%s: %.*s %02d%%%c", title, 100, sign, rate, rate >= 100 ? '\n' : '\r');
    write(STDOUT_FILENO, buffer, n);
}

int main() {
    for (int i = 0; i < 1000; i += 120) {
        Progress("Progress", i / 10);
        usleep(200 * 1000);
    }
    Progress("Progress", 100);
    return 0;
}
