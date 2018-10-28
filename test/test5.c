#ifdef CTRL
# include <stdlib.h>
#else
# include "malloc.h"
#endif

int		main()
{
    malloc(1024);
    malloc(1024 * 32);
    malloc(1024 * 1024);
    malloc(1024 * 1024 * 16);
    malloc(1024 * 1024 * 128);
#ifndef CTRL
    show_alloc_mem();
#endif
	return (0);
}
