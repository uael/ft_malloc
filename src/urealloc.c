/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   urealloc.c                                         :+:      :+:    :+:   */
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

static void	*unlocked_urealloc(t_pool *pool, void *ptr, size_t sz, int zero)
{
	t_chunk	*chk;
	t_bin	*bin;
	void	*nptr;
	size_t	psz;

	if (!(chk = bin_lookup(pool, ptr, &bin)))
		return (NULL);
	if (!bin_resize(bin, chk, sz))
		return (ptr);
	psz = chk->lrg ? bin->size - sizeof(t_bin) - sizeof(t_chunk)
				   : chunk_size(chk);
	if ((nptr = unlocked_ualloc(pool, sz, ALIGN)))
	{
		ft_memcpy(nptr, (void *)chunk_mem(chk), psz < sz ? psz : sz);
		if (zero && sz > psz)
			ft_memset(nptr + psz, 0, sz - psz);
		bin_free(bin, chk);
	}
	return (nptr);
}

void		*urealloc(t_pool *pool, void *ptr, size_t sz)
{
	void	*nptr;

	if (!sz)
		return (NULL);
	if (!ptr)
		return (ualloc(pool, sz));
	if (!pool)
		pool = g_heap_dft;
	pthread_mutex_lock(&pool->lock);
	nptr = unlocked_urealloc(pool, ptr, sz, 0);
	pthread_mutex_unlock(&pool->lock);
	return (nptr);
}

void		*uzrealloc(t_upool pool, void *ptr, size_t sz)
{
	void	*nptr;

	if (!sz)
		return (NULL);
	if (!ptr)
		return (ualloc(pool, sz));
	if (!pool)
		pool = g_heap_dft;
	pthread_mutex_lock(&pool->lock);
	nptr = unlocked_urealloc(pool, ptr, sz, 1);
	pthread_mutex_unlock(&pool->lock);
	return (nptr);
}
