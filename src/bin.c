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
	while (chk->rfc || chunk_size(chk) < sz)
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
	chk = chunk_nxt(nxt, bin);
	chk->prv = nxt->off;
}

void			*bin_flat_alloc(t_bin *bin, size_t sz)
{
	t_chunk	*chk;
	void	*ptr;

	ptr = NULL;
	sz = (sz + sizeof(t_chunk) - 1) & -sizeof(t_chunk);
	if ((chk = find_free(bin, sz)))
	{
		if (chunk_size(chk) > sz + sizeof(t_chunk))
			trim(bin, chk, sz);
		chk->rfc = 1;
		chk->lrg = 0;
		ptr = (void *)chunk_mem(chk);
	}
	return (ptr);
}

void			bin_free(t_bin *bin, t_chunk *chk)
{
	t_chunk	*prv;
	t_chunk	*nxt;

	if (!chk->rfc)
		return;
	if (!--chk->rfc)
	{
		if (!chk->lrg)
		{
			prv = chunk_prv(chk, bin);
			nxt = chunk_nxt(chk, bin);
			if ((prv != chk && !prv->rfc) || !nxt->rfc)
			{
				if (!nxt->rfc)
					nxt = chunk_nxt(nxt, bin);
				if (!prv->rfc)
					chk = prv;
				chk->nxt = nxt->off;
				nxt->prv = chk->off;
			}
		}
		if (chk->lrg || bin->head->nxt == bin->tail->off)
			bin_dyn_free(bin);
	}
}

int				bin_resize(t_bin *bin, t_chunk *chk, size_t nsz)
{
	t_chunk	*nxt;
	size_t	chsz;
	int		ret;

	if (chk->lrg)
		return (-1);
	ret = 0;
	chsz = chunk_size(chk);
	nsz = (nsz + sizeof(t_chunk) - 1) & -sizeof(t_chunk);
	if (chsz < nsz)
	{
		nxt = chunk_nxt(chk, bin);
		if (nxt->rfc || (chunk_size(nxt) + sizeof(t_chunk) + chsz) < nsz)
			ret = -1;
		else
		{
			nxt = chunk_nxt(nxt, bin);
			chk->nxt = nxt->off;
			nxt->prv = chk->off;
			chsz = chunk_size(chk);
		}
	}
	if (!ret && chsz > nsz + sizeof(t_chunk))
		trim(bin, chk, nsz);
	return (ret);
}
