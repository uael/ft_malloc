/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ualloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:42 by alucas-           #+#    #+#             */
/*   Updated: 1970/01/01 00:00:42 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ualloc.h"
#include "../src/bin.h"
#include "../src/pool.h"

#include <assert.h>
#include <string.h>
#include <libft/io.h>

uint8_t g_mem[UINT16_MAX];

void dump(t_pool *pool, t_stream *f, t_bin *bin, t_chunk *chunk, int i)
{
	if (!chunk) return;

 	assert(bin == chunk_bin(chunk));

 	if (i == 0)
	{
		ft_fprintf(f, "         (%6u) |%p -> %p|\n",
			bin->size, bin, bin + bin->size);
	}

	ft_fprintf(f, "%d: [%c] (%6u) |%p -> %p|\n",
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

void dump_pool(t_stream *f, t_pool *pool)
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

int main(int ac, char *av[])
{
	t_upool pool;
	void *ptr[99];

	ft_printf("size: %zu\n", sizeof(t_bin));

	assert(0 == uheap(&pool));

	ptr[0] = ualloc(pool, 64);

	dump_pool(g_stdout, pool);

	ptr[1] = ualloc(pool, 64);

	dump_pool(g_stdout, pool);

	ptr[2] = ualloc(pool, 64);

	dump_pool(g_stdout, pool);

	ptr[3] = ualloc(pool, 64);

	dump_pool(g_stdout, pool);

	ptr[4] = ualloc(pool, 64);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[1]);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[2]);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[3]);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[0]);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[4]);

	dump_pool(g_stdout, pool);

	ptr[0] = ualloc(pool, 250);

	dump_pool(g_stdout, pool);

	ptr[1] = ualloc(pool, 3245);

	dump_pool(g_stdout, pool);

	ptr[2] = ualloc(pool, 64);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[1]);

	dump_pool(g_stdout, pool);

	ptr[1] = ualloc(pool, 3200);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[0]);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[1]);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[2]);

	dump_pool(g_stdout, pool);

	ptr[0] = ualloc(pool, 64);

	dump_pool(g_stdout, pool);

	ptr[1] = ualloc(pool, 64);

	dump_pool(g_stdout, pool);

	ptr[2] = ualloc(pool, 64);

	dump_pool(g_stdout, pool);

	ptr[3] = ualloc(pool, 64);

	dump_pool(g_stdout, pool);

	ptr[4] = ualloc(pool, 64);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[1]);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[2]);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[3]);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[0]);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[4]);

	dump_pool(g_stdout, pool);

	ptr[0] = ualloc(pool, 250);

	dump_pool(g_stdout, pool);

	ptr[1] = ualloc(pool, 3245);

	dump_pool(g_stdout, pool);

	ptr[2] = ualloc(pool, 64);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[1]);

	dump_pool(g_stdout, pool);

	ptr[1] = ualloc(pool, 3200);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[0]);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[1]);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[2]);

	dump_pool(g_stdout, pool);

	ptr[1] = ualloc(pool, 0xffff);

	dump_pool(g_stdout, pool);

	ufree(pool, ptr[1]);

	dump_pool(g_stdout, pool);

	ptr[1] = ualloc(pool, 10);

	dump_pool(g_stdout, pool);

	ptr[1] = urealloc(pool, ptr[1], 20);
	dump_pool(g_stdout, pool);

	ptr[1] = urealloc(pool, ptr[1], 30);
	dump_pool(g_stdout, pool);

	ptr[1] = urealloc(pool, ptr[1], 40);
	dump_pool(g_stdout, pool);

	ptr[2] = ualloc(pool, 64);
	dump_pool(g_stdout, pool);

	ptr[1] = urealloc(pool, ptr[1], 50);
	dump_pool(g_stdout, pool);

	urelease(pool);
	dump_pool(g_stdout, pool);

	return 0;
}