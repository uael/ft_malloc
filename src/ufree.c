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

int			uref(void *ptr)
{
	t_chunk	*chk;

	if (!ptr)
	{
		errno = EINVAL;
		return -1;
	}
	chk = (t_chunk *)ptr - 1;
	if (!chk->refc)
	{
		errno = EINVAL;
		return -1;
	}
	++chk->refc;
	return 0;
}

void		ufree(void *ptr)
{
	t_chunk	*chk;

	if (!ptr)
		return;
	chk = (t_chunk *)ptr - 1;
	bin_free(chunk_bin(chk), chk);
}

size_t		usize(void *ptr)
{
	t_chunk	*chk;

	if (!ptr)
		return (0);
	chk = (t_chunk *)ptr - 1;
	return (chunk_size(chk));
}
