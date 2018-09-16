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

#include "ualloc.h"

#include <assert.h>
#include <string.h>

uint8_t g_mem[128 * 1024];

int main(int ac, char *av[])
{
	t_upool pool;
	void *ptr, *ptr2;
	void *ptrs[60];
	unsigned i;

	ustack(g_mem, sizeof(g_mem), &pool);

	ptr = ualloc(pool, 67);
	memset(ptr, 'a', usize(ptr));
	ufree(ptr);
	ptr2 = ualloc(pool, 68);
	memset(ptr2, 'a', usize(ptr2));
	ufree(ptr2);
	assert(ptr == ptr2);

	for (i = 0; i < 60; ++i)
	{
		ptrs[i] = ualloc(pool, i * i);
		assert(i * i == usize(ptrs[i]));
		memset(ptrs[i], 'a', usize(ptrs[i]));
	}

	while (i)
		ufree(ptrs[--i]);

	return 0;
}