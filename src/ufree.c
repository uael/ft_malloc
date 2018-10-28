/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ufree.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:42 by alucas-           #+#    #+#             */
/*   Updated: 1970/01/01 00:00:42 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pool.h"

#include <errno.h>
#include <malloc.h>

int			uref(t_pool *pool, void *ptr)
{
	t_chunk	*chk;
	t_bin	*bin;

	if (!ptr || (size_t)ptr % sizeof(t_chunk))
	{
		errno = EINVAL;
		return (-1);
	}
	if (!pool)
		pool = g_heap_dft;
	pthread_mutex_lock(&pool->lock);
	if (!(chk = bin_find(pool, ptr, &bin)))
	{
		pthread_mutex_unlock(&pool->lock);
		return (-1);
	}
	if (!chk->rfc)
	{
		errno = EINVAL;
		pthread_mutex_unlock(&pool->lock);
		return (-1);
	}
	++chk->rfc;
	pthread_mutex_unlock(&pool->lock);
	return (0);
}

void		ufree(t_pool *pool, void *ptr)
{
	t_chunk	*chk;
	t_bin	*bin;

	if (!ptr || (size_t)ptr % sizeof(t_chunk))
		return ;
	if (!pool)
		pool = g_heap_dft;
	pthread_mutex_lock(&pool->lock);
	if (!(chk = bin_find(pool, ptr, &bin)))
	{
		pthread_mutex_unlock(&pool->lock);
		return ;
	}
	bin_free(bin, chk);
	pthread_mutex_unlock(&pool->lock);
}

size_t		usize(t_pool *pool, void *ptr)
{
	t_chunk	*chk;
	t_bin	*bin;
	size_t	ret;

	if (!ptr || (size_t)ptr % sizeof(t_chunk))
		return (0);
	if (!pool)
		pool = g_heap_dft;
	pthread_mutex_lock(&pool->lock);
	if (!(chk = bin_find(pool, ptr, &bin)))
	{
		pthread_mutex_unlock(&pool->lock);
		return (0);
	}
	ret = chk->lrg ? bin->size - sizeof(t_bin) - sizeof(t_chunk)
				   : chunk_size(chk);
	pthread_mutex_unlock(&pool->lock);
	return (ret);
}
