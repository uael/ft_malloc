/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_dyn.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:42 by alucas-           #+#    #+#             */
/*   Updated: 1970/01/01 00:00:42 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pool.h"

# include <errno.h>
# include <sys/mman.h>

static t_bin			g_bins[MAX_BIN];
static pthread_mutex_t	bin_lock = PTHREAD_MUTEX_INITIALIZER;

static t_bin			*bin_slot(void)
{
	unsigned	i;
	t_bin		*bin;

	i = 0;
	pthread_mutex_lock(&bin_lock);
	while (i < MAX_BIN && g_bins[i].tail)
		++i;
	if (i == MAX_BIN)
	{
		errno = ENOMEM;
		pthread_mutex_unlock(&bin_lock);
		return (NULL);
	}
	bin = g_bins + i;
	pthread_mutex_unlock(&bin_lock);
	return bin;
}

static int				bin_mmap(t_bin *bin, enum e_class class, size_t sz)
{
	void *mem;
	uint16_t tail;

	*bin = (t_bin){ .lock = PTHREAD_MUTEX_INITIALIZER };
	bin->size = class == CLASS_LARGE
		? ((sz + sizeof(void *) + (sizeof(t_chunk) * 2)) - 1) & -sizeof(t_chunk)
		: ((size_t)(1 << class) * MIN_ALLOC);
	mem = mmap(NULL, bin->size, PROT_READ | PROT_WRITE,
			   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (mem == MAP_FAILED)
		return (-1);
	*(void **)mem = bin;
	bin->head = (t_chunk *)((void **)mem + 1);
	tail = (uint16_t)((bin->size - sizeof(void *)) / sizeof(t_chunk));
	if (class != CLASS_LARGE)
		--tail;
	bin->tail = bin->head + tail;
	*bin->tail = (t_chunk){ .off = tail, .refc = 1 };
	*bin->head = (t_chunk){ .nxt = tail };
	return 0;
}

void					*bin_dyn_alloc(t_bin **pbin, enum e_class cl, size_t sz)
{
	void *ptr;
	t_bin *prev;

	prev = NULL;
	while (1)
		if ((!*pbin || !(*pbin)->head)) {
			if (*pbin) (*pbin)->tail = NULL;
			if (!(*pbin = bin_slot()))
				return (NULL);
			(*pbin)->prev = prev;
			return (bin_mmap(*pbin, cl, sz)
					? (*pbin = NULL) : bin_flat_alloc(*pbin, sz));
		}
		else if ((ptr = bin_flat_alloc(*pbin, sz)))
			return (ptr);
		else
		{
			prev = *pbin;
			pbin = &(*pbin)->next;
		}
}

void					bin_dyn_free(t_bin *bin)
{
	munmap(*((void **)bin->head - 1), bin->size);
	bin->head = NULL;
}

void					bin_dyn_freeall(t_bin *bin)
{
	if (!bin)
		return;
	if (bin->head)
		bin_dyn_free(bin);
	return (bin_dyn_freeall(bin->next));
}
