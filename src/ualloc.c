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

#include "config.h"

#include <string.h>
#include <unistd.h>

static void		*mapper_map(size_t size, size_t *off)
{
	(void)size;
	(void)off;
	return (NULL);
}

static void		mapper_unmap(void *addr, size_t size, size_t off, size_t rela)
{
	(void)addr;
	(void)size;
	(void)off;
	(void)rela;
}

struct s_ualloc	g_ualloc = {
	.mapper = {
		.page_size = 0,
		.span_size = UALLOC_SPAN_SIZE,
		.map = mapper_map,
		.unmap = mapper_unmap,
	}
};

void			ualloc_init(const struct s_ualloc_mapper *mapper)
{
	size_t span_size;

	if (mapper)
		memcpy(&g_ualloc.mapper, mapper, sizeof(struct s_ualloc_mapper));
	if (!g_ualloc.mapper.map || !g_ualloc.mapper.unmap)
	{
		g_ualloc.mapper.map = mapper_map;
		g_ualloc.mapper.unmap = mapper_unmap;
	}
	if (!g_ualloc.mapper.page_size)
		g_ualloc.mapper.page_size = getpagesize();
	span_size = g_ualloc.mapper.span_size;
	if (!span_size)
		span_size = UALLOC_SPAN_SIZE;
	if (span_size > UALLOC_MAX_SPAN_SIZE)
		span_size = UALLOC_MAX_SPAN_SIZE;
	g_ualloc.mapper.span_size = 4096;
	g_ualloc.span_size_shift = 12;
	while (g_ualloc.mapper.span_size < span_size)
	{
		g_ualloc.mapper.span_size <<= 1;
		++g_ualloc.span_size_shift;
	}
}

void			ualloc_destroy(void)
{
}
