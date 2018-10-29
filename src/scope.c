/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scope.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:42 by alucas-           #+#    #+#             */
/*   Updated: 1970/01/01 00:00:42 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pool.h"

#include <errno.h>

int		upush(t_upool pool)
{
	pool->prev = g_uscope;
	g_uscope = pool;
	return (0);
}

int		upop(void)
{
	if (!g_uscope->prev)
		return (-(errno = EINVAL));
	urelease(g_uscope);
	g_uscope = g_uscope->prev;
	return (0);
}
