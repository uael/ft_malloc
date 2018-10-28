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

void		*ualloc(t_pool *pool, size_t sz)
{
	enum e_class	class;

	if (!pool)
		pool = g_heap_dft;
	else if (pool->kind == POOL_NONE)
	{
		errno = EINVAL;
		return (NULL);
	}
	if (pool->kind == POOL_STACK)
		return (bin_flat_alloc(&pool->def.stack.bin, sz));
	class = classof(sz);
	if (class == CLASS_TINY)
		return (bin_dyn_alloc(&pool->def.heap.bins_tiny, class, sz));
	if (class == CLASS_SMALL)
		return (bin_dyn_alloc(&pool->def.heap.bins_small, class, sz));
	return (bin_dyn_alloc(&pool->def.heap.bins_large, class, sz));
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

	if (!ptr)
		return (ualloc(pool, sz));
	chk = (t_chunk *)ptr - 1;
	bin = chunk_bin(chk);
	if (!bin_resize(bin, chk, sz))
		return (ptr);
	psz = chunk_size(chk);
	bin_free(bin, chk);
	if (!(nptr = ualloc(pool, sz)))
		return (NULL);
	ft_memcpy(nptr, ptr, psz);
	return (nptr);
}

void		*uzrealloc(t_upool pool, void *ptr, size_t sz)
{
	t_chunk	*chk;
	t_bin	*bin;
	void	*nptr;
	size_t	psz;

	if (!ptr)
		return (ualloc(pool, sz));
	chk = (t_chunk *)ptr - 1;
	bin = chunk_bin(chk);
	if (!bin_resize(bin, chk, sz))
		return (ptr);
	psz = chunk_size(chk);
	bin_free(bin, chk);
	if (!(nptr = ualloc(pool, sz)))
		return (NULL);
	ft_memcpy(nptr, ptr, psz);
	ft_memset(nptr + psz, 0, usize(nptr) - psz);
	return (nptr);
}
