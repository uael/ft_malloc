/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:42 by alucas-           #+#    #+#             */
/*   Updated: 1970/01/01 00:00:42 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "pool.h"

#include <stdio.h>
#include <libft/io.h>
#include <assert.h>

void	free(void *ptr)
{
	ufree(NULL, ptr);
}

void	*malloc(size_t sz)
{
	return (ualloc(NULL, sz));
}

void	*realloc(void *ptr, size_t sz)
{
	return (urealloc(NULL, ptr, sz));
}

void	*reallocf(void *ptr, size_t sz)
{
	return (urealloc(NULL, ptr, sz));
}

void	*calloc(size_t nb, size_t sz)
{
	return (ucalloc(NULL, nb, sz));
}

void	*valloc(size_t sz)
{
	return (ualloc(NULL, sz));
}

static void dump(t_pool *pool, t_stream *f, t_bin *bin, t_chunk *chunk, int i)
{
	if (!chunk) return;

	assert(bin == chunk_bin(chunk));

	if (i == 0)
	{
		ft_fprintf(f, "         (%6u) |%p -> %p|\n",
				   bin->size, bin, bin + bin->size);
	}

	ft_fprintf(f, "%.3d: [%c] (%6u) |%p -> %p|\n",
			   i, chunk->rfc ? 'X' : ' ',
			   usize(pool, (void *)chunk_mem(chunk)), chunk, bin->head + chunk->nxt);
	if (chunk->lrg)
		return;
	if (chunk->nxt == bin->tail->off)
	{
		if (bin->next)
			return dump(pool, f, bin->next, bin->next->head, 0);
		return;
	}
	dump(pool, f, bin, bin->head + chunk->nxt, i + 1);
}

static void dump_pool(t_stream *f, t_pool *pool)
{
	ft_fprintf(f, "-------------------- tiny  -----------------\n");
	if (pool->def.heap.bins_tiny && pool->def.heap.bins_tiny->head)
		dump(pool, f, pool->def.heap.bins_tiny, pool->def.heap.bins_tiny->head, 0);
	ft_fprintf(f, "-------------------- small -----------------\n");
	if (pool->def.heap.bins_small && pool->def.heap.bins_small->head)
		dump(pool, f, pool->def.heap.bins_small, pool->def.heap.bins_small->head, 0);
	ft_fprintf(f, "-------------------- large -----------------\n");
	if (pool->def.heap.bins_large && pool->def.heap.bins_large->head)
		dump(pool, f, pool->def.heap.bins_large, pool->def.heap.bins_large->head, 0);
	ft_fprintf(f, "\n\n");
}

void	show_alloc_mem(void)
{
	dump_pool(g_stdout, g_heap_dft);
}
