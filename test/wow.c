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

void dump(t_stream *f, t_bin *bin, t_chunk *chunk)
{
	if (!chunk) return;

 	assert(bin == chunk_bin(chunk));

	ft_fprintf(f, "[%c] (%6u) |%p -> %p|\n",
			   chunk->refc ? 'X' : ' ',
			   chunk_size(chunk), chunk, bin->head + chunk->nxt);
	if (chunk->nxt == bin->tail->off)
	{
		if (bin->next)
			return dump(f, bin->next, bin->next->head);
		return;
	}
	dump(f, bin, bin->head + chunk->nxt);
}

void dump_pool(t_stream *f, t_pool *pool)
{
	ft_fprintf(g_stdout, "-------------------- tiny  -----------------\n");
	if (pool->def.heap.bins_tiny && pool->def.heap.bins_tiny->head)
		dump(g_stdout, pool->def.heap.bins_tiny, pool->def.heap.bins_tiny->head);
	ft_fprintf(g_stdout, "-------------------- small -----------------\n");
	if (pool->def.heap.bins_small && pool->def.heap.bins_small->head)
		dump(g_stdout, pool->def.heap.bins_small, pool->def.heap.bins_small->head);
	ft_fprintf(g_stdout, "-------------------- large -----------------\n");
	if (pool->def.heap.bins_large && pool->def.heap.bins_large->head)
		dump(g_stdout, pool->def.heap.bins_large, pool->def.heap.bins_large->head);
	ft_fprintf(g_stdout, "\n\n");
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

	ufree(ptr[1]);

	dump_pool(g_stdout, pool);

	ufree(ptr[2]);

	dump_pool(g_stdout, pool);

	ufree(ptr[3]);

	dump_pool(g_stdout, pool);

	ufree(ptr[0]);

	dump_pool(g_stdout, pool);

	ufree(ptr[4]);

	dump_pool(g_stdout, pool);

	ptr[0] = ualloc(pool, 250);

	dump_pool(g_stdout, pool);

	ptr[1] = ualloc(pool, 3245);

	dump_pool(g_stdout, pool);

	ptr[2] = ualloc(pool, 64);

	dump_pool(g_stdout, pool);

	ufree(ptr[1]);

	dump_pool(g_stdout, pool);

	ptr[1] = ualloc(pool, 3200);

	dump_pool(g_stdout, pool);

	ufree(ptr[0]);

	dump_pool(g_stdout, pool);

	ufree(ptr[1]);

	dump_pool(g_stdout, pool);

	ufree(ptr[2]);

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

	ufree(ptr[1]);

	dump_pool(g_stdout, pool);

	ufree(ptr[2]);

	dump_pool(g_stdout, pool);

	ufree(ptr[3]);

	dump_pool(g_stdout, pool);

	ufree(ptr[0]);

	dump_pool(g_stdout, pool);

	ufree(ptr[4]);

	dump_pool(g_stdout, pool);

	ptr[0] = ualloc(pool, 250);

	dump_pool(g_stdout, pool);

	ptr[1] = ualloc(pool, 3245);

	dump_pool(g_stdout, pool);

	ptr[2] = ualloc(pool, 64);

	dump_pool(g_stdout, pool);

	ufree(ptr[1]);

	dump_pool(g_stdout, pool);

	ptr[1] = ualloc(pool, 3200);

	dump_pool(g_stdout, pool);

	ufree(ptr[0]);

	dump_pool(g_stdout, pool);

	ufree(ptr[1]);

	dump_pool(g_stdout, pool);

	ufree(ptr[2]);

	dump_pool(g_stdout, pool);

	ptr[1] = ualloc(pool, 0xffff);

	dump_pool(g_stdout, pool);

	ufree(ptr[1]);

	dump_pool(g_stdout, pool);

	return 0;
}