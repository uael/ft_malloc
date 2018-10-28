#ifdef CTRL
# include <stdlib.h>
#else
# include "malloc.h"
#endif
#include <unistd.h>
#include <string.h>

#define P (4096)

int		main()
{
    size_t i;
    char *addr1[P];
    char *addr2[P];

    for (i = 0; i < P; i++) {
        addr1[i] = malloc((i + 1) * 64);
        if (i % 3) {
            free(addr1[i]);
        }
    }

    for (i = 0; i < P; i++) {
        addr2[i] = malloc((i + 1) * 64);
    }

#ifndef CTRL
    for (i = 1; i < P; i++) {
        free((void *)((size_t)addr2[P - 2] + i));
    }
#endif

    for (i = 0; i < P; i++) {
        if (!(i % 3)) {
            free(addr1[i]);
        }
    }

    for (i = 0; i < P; i++) {
        free(addr2[i]);
    }

	return (0);
}
