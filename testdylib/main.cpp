#include <cstdio>

void install(void) __attribute__ ((constructor));

void install()
{
    printf("hello, world!\n");
}


void payload_entry() {
    printf("in payload_entry!!\n");
}
