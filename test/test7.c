#include <string.h>
#ifdef CTRL
# include <stdlib.h>
#else
# include "malloc.h"
#endif

int		main()
{
    memcpy(malloc(42), "hello\n\xff\x0\x1", 9);
#ifndef CTRL
    show_alloc_mem();
#endif
	return (0);
}
