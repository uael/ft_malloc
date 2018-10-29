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

#include "pool.h"

#include <errno.h>
#include <libft.h>

void		*unlocked_ualloc(t_pool *pool, size_t sz, size_t al)
{
	enum e_class	class;
	void			*ptr;

	class = classof(sz);
	if (pool->kind == POOL_STACK)
		ptr = bin_alloc(pool->def.stack.bin, sz, al);
	else if (class == CLASS_TINY)
		ptr = bin_dyalloc(&pool->def.heap.bins_tiny, class, sz, al);
	else if (class == CLASS_SMALL)
		ptr = bin_dyalloc(&pool->def.heap.bins_small, class, sz, al);
	else
		ptr = bin_dyalloc(&pool->def.heap.bins_large, class, sz, al);
	return (ptr);
}

void		*ualloc(t_pool *pool, size_t sz)
{
	void *ptr;

	if (!sz)
		return (NULL);
	if (!pool)
		pool = g_heap_dft;
	else if (pool->kind == POOL_NONE)
	{
		errno = EINVAL;
		return (NULL);
	}
	pthread_mutex_lock(&pool->lock);
	ptr = unlocked_ualloc(pool, sz, ALIGN);
	pthread_mutex_unlock(&pool->lock);
	return (ptr);
}

void		*uzalloc(t_pool *pool, size_t sz)
{
	void *ptr;

	if ((ptr = ualloc(pool, sz)))
		ft_memset(ptr, 0, sz);
	return (ptr);
}

void		*ucalloc(t_pool *pool, size_t num, size_t sz)
{
	return (uzalloc(pool, num * sz));
}
