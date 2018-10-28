#ifdef CTRL
# include <stdlib.h>
#else
# include "malloc.h"
#endif

int		main()
{
    int     i;
    /* char    *addr; */

    i = 0;
    while (i < 1024)
    {
        i++;
    }
	return (0);
}
