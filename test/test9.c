#include <string.h>
#include <printf.h>
#include <ualloc.h>

#ifdef CTRL
# include <stdlib.h>
#else
# include "malloc.h"
#endif

static char *array[64 * 64];

int		main(void)
{
	size_t i;

	i = 0;
	while (i < 8)
	{
		array[i] = calloc(i * i, 1);
		printf("usize: %zu\n", usize(NULL, array[i]));
		i++;
	}
	i = 0;
	show_alloc_mem();
	while (i < 8)
	{
		array[i] = realloc(array[i], (i * i) + 10);
		i++;
		show_alloc_mem();
	}
	show_alloc_mem();
	i = 0;
	while (i < 8)
	{
		free(array[i]);
		i++;
	}
	show_alloc_mem();
	return (0);
}
