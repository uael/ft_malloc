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
#include <malloc.h>
#include <assert.h>

void		*ualloc(t_pool *pool, size_t sz)
{
	enum e_class	class;
	void			*ptr;


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
	class = classof(sz);
	if (pool->kind == POOL_STACK)
		ptr = bin_flat_alloc(pool->def.stack.bin, sz);
	else if (class == CLASS_TINY)
		ptr = bin_dyn_alloc(&pool->def.heap.bins_tiny, class, sz);
	else if (class == CLASS_SMALL)
		ptr = bin_dyn_alloc(&pool->def.heap.bins_small, class, sz);
	else
		ptr = bin_dyn_alloc(&pool->def.heap.bins_large, class, sz);
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

void		*urealloc(t_pool *pool, void *ptr, size_t sz)
{
	t_chunk	*chk;
	t_bin	*bin;
	void	*nptr;
	size_t	psz;

	if (!sz)
		return (NULL);
	if (!ptr)
		return (ualloc(pool, sz));
	if (!pool)
		pool = g_heap_dft;
	pthread_mutex_lock(&pool->lock);
	if (!(chk = bin_find(pool, ptr, &bin)))
	{
		pthread_mutex_unlock(&pool->lock);
		return (NULL);
	}
	if (!bin_resize(bin, chk, sz))
	{
		pthread_mutex_unlock(&pool->lock);
		return (ptr);
	}
	psz = chk->lrg ? bin->size - sizeof(t_bin) - sizeof(t_chunk)
		: chunk_size(chk);
	pthread_mutex_unlock(&pool->lock);
	if ((nptr = ualloc(pool, sz)))
	{
		ft_memcpy(nptr, (void *)chunk_mem(chk), psz < sz ? psz : sz);
		bin_free(bin, chk);
	}
	return (nptr);
}

void		*uzrealloc(t_upool pool, void *ptr, size_t sz)
{
	t_chunk	*chk;
	t_bin	*bin;
	void	*nptr;
	size_t	psz;

	if (!sz)
		return (NULL);
	if (!ptr)
		return (ualloc(pool, sz));
	if (!pool)
		pool = g_heap_dft;
	pthread_mutex_lock(&pool->lock);
	if (!(chk = bin_find(pool, ptr, &bin)))
	{
		pthread_mutex_unlock(&pool->lock);
		return (NULL);
	}
	if (!bin_resize(bin, chk, sz))
	{
		pthread_mutex_unlock(&pool->lock);
		return (ptr);
	}
	psz = chunk_size(chk);
	pthread_mutex_unlock(&pool->lock);
	if (!(nptr = ualloc(pool, sz)))
		return (NULL);
	ft_memcpy(nptr, (void *)chunk_mem(chk), psz < sz ? psz : sz);
	if (sz > psz)
		ft_memset(nptr + psz, 0, usize(pool, nptr) - psz);
	bin_free(bin, chk);
	return (nptr);
}
