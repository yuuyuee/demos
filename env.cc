#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

extern char** environ;

int main(void) {
    size_t count;
    for (count = 0; environ[count]; ++count)
        ;
    printf("After environ size: %zd %p %p\n", count, environ, environ[0]);
    char item[100];
    memcpy(item, "name=yuyue", sizeof("name=yuyue"));
    putenv(item);
    for (count = 0; environ[count]; ++count)
        ;
    printf("After environ size: %zd %p %p\n", count, environ, environ[0]);
    return 0;
}
