/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin.c                                              :+:      :+:    :+:   */
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

static t_chunk	*find_free(t_bin *bin, size_t sz)
{
	t_chunk	*chk;

	chk = bin->head;
	while (chk->refc || chunk_size(chk) < sz)
	{
		if (chk->nxt == bin->tail->off)
			return NULL;
		chk = chunk_nxt(chk, bin);
	}
	return (chk);
}

static void		trim(t_bin *bin, t_chunk *chk, size_t sz)
{
	t_chunk	*nxt;

	nxt = (t_chunk *)(chunk_mem(chk) + sz);
	*nxt = (t_chunk){ .nxt = chk->nxt, .prv = chk->off,
		.off = (uint16_t)(nxt - bin->head) };
	chk->nxt = nxt->off;
	chk = nxt;
	nxt = chunk_nxt(chk, bin);
	if (!nxt->refc)
	{
		chk->nxt = nxt->off;
		nxt->prv = chk->off;
	}
}

void			*bin_flat_alloc(t_bin *bin, size_t sz)
{
	t_chunk	*chk;
	void	*ptr;

	ptr = NULL;
	sz = (sz + sizeof(t_chunk) - 1) & -sizeof(t_chunk);
	pthread_mutex_lock(&bin->lock);
	if ((chk = find_free(bin, sz)))
	{
		if (chunk_size(chk) > sz + sizeof(t_chunk))
			trim(bin, chk, sz);
		chk->refc = 1;
		chk->solo = 0;
		ptr = (void *)chunk_mem(chk);
	}
	pthread_mutex_unlock(&bin->lock);
	return (ptr);
}

void			bin_free(t_bin *bin, t_chunk *chk)
{
	t_chunk	*prv;
	t_chunk	*nxt;

	if (!chk->refc)
		return;
	pthread_mutex_lock(&bin->lock);
	if (!--chk->refc)
	{
		prv = chunk_prv(chk, bin);
		nxt = chunk_nxt(chk, bin);
		if (!prv->refc || !nxt->refc)
		{
			if (!nxt->refc)
				nxt = chunk_nxt(nxt, bin);
			if (!prv->refc)
				chk = prv;
			chk->nxt = nxt->off;
			nxt->prv = chk->off;
		}
		if (bin->head->nxt == bin->tail->off)
			bin_dyn_free(bin);
	}
	pthread_mutex_unlock(&bin->lock);
}

int				bin_resize(t_bin *bin, t_chunk *chk, size_t nsz)
{
	t_chunk	*nxt;
	size_t	chsz;

	chsz = chunk_size(chk);
	nsz = (nsz + sizeof(t_chunk) - 1) & -sizeof(t_chunk);
	if (chsz > nsz + sizeof(t_chunk))
	{
		trim(bin, chk, nsz);
		return (0);
	}
	if (chsz >= nsz)
		return (0);
	nxt = chunk_nxt(chk, bin);
	if (nxt->refc || (chunk_size(nxt) + sizeof(t_chunk) + chsz) < nsz)
		return (1);
	chk->nxt = nxt->off;
	nxt->prv = chk->off;
	return (0);
}
