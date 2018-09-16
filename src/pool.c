/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:42 by alucas-           #+#    #+#             */
/*   Updated: 1970/01/01 00:00:42 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pool.h"

#include <unistd.h>
#include <errno.h>

struct s_upool	g_pools[MAX_POOL];

int				ustack(void *mem, size_t sz, t_upool *pool)
{
	unsigned pool_id;

	pool_id = 0;
	while (pool_id < MAX_POOL && g_pools[pool_id].free)
		++pool_id;
	if (pool_id == MAX_POOL)
	{
		errno = ENOMEM;
		return (-1);
	}
	g_pools[pool_id].memory = mem;
	g_pools[pool_id].size = sz;
	g_pools[pool_id].free = mem;
	g_pools[pool_id].free->size = (uint32_t)(sz - sizeof(struct s_uptr));
	*pool = g_pools + pool_id;
	return 0;
}
