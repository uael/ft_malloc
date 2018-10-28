/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:42 by alucas-           #+#    #+#             */
/*   Updated: 1970/01/01 00:00:42 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pool.h"

#include <errno.h>

t_pool			g_pools[MAX_POOL];
t_pool			g_heap_dft_stack = { .kind = POOL_HEAP };
pthread_mutex_t	g_pool_lock = PTHREAD_MUTEX_INITIALIZER;

t_pool			*g_heap_dft = &g_heap_dft_stack;

static t_pool	*pool_slot(enum e_pool kind)
{
	unsigned	i;
	t_pool		*pool;

	i = 0;
	pthread_mutex_lock(&g_pool_lock);
	while (i < MAX_POOL && g_pools[i].kind)
		++i;
	if (i == MAX_POOL)
	{
		errno = ENOMEM;
		pthread_mutex_unlock(&g_pool_lock);
		return (NULL);
	}
	pool = g_pools + i;
	pool->kind = kind;
	pthread_mutex_init(&pool->lock, NULL);
	pthread_mutex_unlock(&g_pool_lock);
	return (pool);
}

static void		stack_init(t_pool *pool, void *mem, size_t sz)
{
	t_bin		*bin;
	uint16_t	tail;

	pool->def.stack.bin = mem;
	bin = pool->def.stack.bin;
	*bin = (t_bin){
		.size = sz - sizeof(t_bin) };
	tail = (uint16_t)(bin->size / sizeof(t_chunk));
	bin->head = (t_chunk *)((t_bin *)mem + 1);
	bin->tail = bin->head + tail;
	*bin->tail = (t_chunk){ .off = tail, .rfc = 1 };
	*bin->head = (t_chunk){ .nxt = tail };
}

int				ustack(void *mem, size_t sz, t_pool **ppool)
{
	t_pool		*pool;

	if (sz < sizeof(t_bin))
	{
		errno = EINVAL;
		return (-1);
	}
	if (!(pool = pool_slot(POOL_STACK)))
		return (-1);
	stack_init(pool, mem, sz);
	*ppool = pool;
	return (0);
}

int				uheap(t_pool **ppool)
{
	t_pool *pool;

	if (!(pool = pool_slot(POOL_HEAP)))
		return (-1);
	*ppool = pool;
	return (0);
}

int				urelease(t_upool pool)
{
	pthread_mutex_t *mutex;

	if (pool->kind == POOL_NONE)
	{
		errno = EINVAL;
		return (-1);
	}
	pthread_mutex_lock(mutex = &pool->lock);
	if (pool->kind == POOL_STACK)
	{
		stack_init(pool, pool->def.stack.bin,
			pool->def.stack.bin->size + sizeof(t_bin));
		pthread_mutex_unlock(mutex);
		return (0);
	}
	bin_dyn_freeall(pool->def.heap.bins_tiny);
	pool->def.heap.bins_tiny = NULL;
	bin_dyn_freeall(pool->def.heap.bins_small);
	pool->def.heap.bins_small = NULL;
	bin_dyn_freeall(pool->def.heap.bins_large);
	pool->def.heap.bins_large = NULL;
	if (pool != g_heap_dft)
		pool->kind = POOL_NONE;
	pthread_mutex_unlock(mutex);
	if (pool != g_heap_dft)
		pthread_mutex_destroy(mutex);
	return (0);
}
