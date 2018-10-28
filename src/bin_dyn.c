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

static int		bin_mmap(t_bin **pbin, t_bin **bhd,
								 enum e_class class, size_t sz)
{
	void		*mem;
	t_bin		*bin;
	uint16_t	tail;

	sz = class == CLASS_LARGE
		? (sz + sizeof(t_bin) + sizeof(t_chunk) - 1) & -sizeof(t_chunk)
		: ((size_t)(1 << class) * MIN_ALLOC);
	mem = mmap(NULL, sz, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (mem == MAP_FAILED)
	{
		*pbin = NULL;
		return (-1);
	}
	*pbin = mem;
	bin = *pbin;
	*bin = (t_bin){ .bhd = bhd, .size = sz };
	bin->head = (t_chunk *)((t_bin *)mem + 1);
	if (class == CLASS_LARGE)
		*bin->head = (t_chunk){ .lrg = 1, .rfc = 1 };
	else
	{
		tail = (uint16_t)(((bin->size - sizeof(t_bin)) / sizeof(t_chunk)) - 1);
		bin->tail = bin->head + tail;
		*bin->tail = (t_chunk){ .off = tail };
		*bin->head = (t_chunk){ .nxt = tail };
	}
	return 0;
}

void			*bin_dyn_alloc(t_bin **pbin, enum e_class cl, size_t sz)
{
	void	*ptr;
	t_bin	*prev;
	t_bin	**bhd;

	bhd = pbin;
	prev = NULL;
	while (1)
		if (!*pbin)
		{
			if (bin_mmap(pbin, bhd, cl, sz))
				return (NULL);
			(*pbin)->prev = prev;
			return (cl == CLASS_LARGE
				? (*pbin)->head + 1 : bin_flat_alloc(*pbin, sz));
		}
		else if (cl != CLASS_LARGE && (ptr = bin_flat_alloc(*pbin, sz)))
			return (ptr);
		else
		{
			prev = *pbin;
			pbin = &(*pbin)->next;
		}
}

void			bin_dyn_free(t_bin *bin)
{
	if (bin->prev)
		bin->prev->next = bin->next;
	if (bin->next)
		bin->next->prev = bin->prev;
	if (bin == *bin->bhd)
		*bin->bhd = bin->next;
	munmap(bin, bin->size);
}

void			bin_dyn_freeall(t_bin *bin)
{
	t_bin *next;

	if (!bin)
		return;
	next = bin->next;
	bin_dyn_free(bin);
	return (bin_dyn_freeall(next));
}

static t_chunk	*find_alloc(t_bin *bin, uintptr_t ptr, t_bin **pbin)
{
	t_chunk	*chk;

	chk = bin->head;
	while (1)
	{
		if (chunk_mem(chk) >= ptr && ptr < (uintptr_t)chunk_nxt(chk, bin))
		{
			*pbin = bin;
			return (chk);
		}
		if (chk->nxt == bin->tail->off)
			break;
		chk = chunk_nxt(chk, bin);
	}
	return (NULL);
}

t_chunk			*bin_fnd(int lrg, t_bin *bin, uintptr_t ptr, t_bin **pbin)
{
	if (!bin)
		return (NULL);
	if (ptr >= (uintptr_t)bin && ptr <= ((uintptr_t)bin + bin->size))
	{
		if (lrg)
		{
			*pbin = bin;
			return (bin->head);
		}
		return (find_alloc(bin, ptr, pbin));
	}
	return (bin_fnd(lrg, bin->next, ptr, pbin));
}

t_chunk			*bin_find(struct s_pool *pool, void *ptr, t_bin **pbin)
{
	t_chunk *chk;

	if (pool->kind == POOL_STACK)
		return (bin_fnd(0, pool->def.stack.bin, (uintptr_t)ptr, pbin));
	if ((chk = bin_fnd(0, pool->def.heap.bins_tiny, (uintptr_t)ptr, pbin)))
		return (chk);
	if ((chk = bin_fnd(0, pool->def.heap.bins_small, (uintptr_t)ptr, pbin)))
		return (chk);
	return (bin_fnd(1, pool->def.heap.bins_large, (uintptr_t)ptr, pbin));
}
