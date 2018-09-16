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

#include <string.h>
#include <unistd.h>
#include <errno.h>

void				*ualloc(t_upool pool, size_t sz)
{
	struct s_uptr	*ptr;
	struct s_uptr	*next;

	ptr = pool->free;
	while (ptr && ptr->next && ptr->size < sz)
		ptr = ptr->next;
	if (!ptr || ptr->size < sz)
	{
		errno = ENOMEM;
		return (NULL);
	}
	if (ptr->size <= sz + sizeof(struct s_uptr))
		next = ptr->next;
	else
	{
		next = (struct s_uptr *)((uint8_t *)(ptr + 1) + sz);
		next->size = (uint32_t)(ptr->size - ((char *)next - (char *)ptr));
		next->next = ptr->next;
	}
	if (pool->free == ptr)
		pool->free = next;
	ptr->pool = pool;
	ptr->size = (uint32_t)sz;
	ptr->refcnt = 1;
	return ptr + 1;
}

void				ufree(void *ptr)
{
	struct s_uptr	*hdr;
	t_upool			pool;

	hdr = (struct s_uptr *)ptr - 1;
	if (--hdr->refcnt)
		return;
	pool = hdr->pool;
	hdr->next = pool->free;
	pool->free = hdr;
	if ((char *)(hdr + 1) + hdr->size == (char *)hdr->next)
	{
		hdr->size += hdr->next->size + sizeof(struct s_uptr);
		hdr->next = hdr->next->next;
	}
}

size_t				usize(void *ptr)
{
	struct s_uptr	*hdr;

	hdr = (struct s_uptr *)ptr - 1;
	return hdr->size;
}
