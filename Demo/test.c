#include <stdio.h>

int main(int argc, char *argv[])
{
    fprintf(stdout, "Hello, World! stdout\n");
    fprintf(stderr, "Hello, World! stderr\n");
    // fprintf(stdout, argv[1]);
    return 0;
}

