#include <string.h>
#include <printf.h>
#include <ualloc.h>

#ifdef CTRL
# include <stdlib.h>
#else
# include "malloc.h"
#endif

static char *array[1024 * 1024];

int		main(void)
{
	size_t i;

	i = 0;
	while (i < 1024 * 1024)
	{
		array[i] = calloc(i, 1);
		i++;
	}
	i = 0;
	while (i < 1024 * 1024)
	{
		array[i] = realloc(array[i], (i) + 10);
		i++;
	}
	i = 0;
	while (i < 1024 * 1024)
	{
		free(array[i]);
		i++;
	}
	show_alloc_mem();
	return (0);
}
