#include <unistd.h>
#include "ackerman.h"
#include "stdio.h"

int main(int argc, char ** argv) {

    // input parameters (parse program args)
    int b = 128;
    int s = 512000;
    int c;
    int errflag = 0;
    while ((c = getopt(argc, argv, "::b::s::")) != -1) {
        switch (c) {
        case 'b':
            b = atoi(optarg);
            break;
        case 's':
            s = atoi(optarg);
            break;
        case ':':
            fprintf(stderr, "Option -%c requires an operand\n", optopt);
            ++errflag;
            break;
        case '?':
            fprintf(stderr, "Unrecognised option: -%c\n", optopt);
            ++errflag;
            break;
        }
    }
    if (errflag) {
        fprintf(stderr, "usage: memtest [-b <blocksize>] [-s <memsize>]\n");
        return 1;
    }

    if (b <= 0 || s <= 0) {
        fprintf(stderr, "Arguments must be positive");
        return 1;
    }
    
    if (s % b != 0) {
        fprintf(stderr, "Memsize must be divisible by blocksize");
        return 1;
    }

    init_allocator(b, s);

    ackerman_main();

    release_allocator();
}
