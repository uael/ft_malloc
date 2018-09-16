/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:42 by alucas-           #+#    #+#             */
/*   Updated: 1970/01/01 00:00:42 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POOL_H
# define POOL_H

# include "ualloc.h"

#define MAX_POOL 16

struct				s_uptr
{
	struct s_uptr	*next;
	t_upool			pool;
	uint32_t		size;
	uint32_t		refcnt;
} __attribute__((__packed__));

extern struct		s_upool
{
	t_upool			prev;
	t_upool			next;
	void			*memory;
	size_t			size;
	void			*(*map)(size_t sz);
	void			(*unmap)(void *memory);
	struct s_uptr	*free;
}					g_pools[];

#endif
