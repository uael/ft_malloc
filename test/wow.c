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
#include "../src/bin.h"
#include "../src/pool.h"

#include <assert.h>
#include <string.h>
#include <libft/io.h>

uint8_t g_mem[UINT32_MAX];

int main(int ac, char *av[])
{
	t_upool pool;
	void *ptr[99];

	ft_printf("size: %zu\n", sizeof(t_bin));

	assert(0 == ustack(g_mem, sizeof(g_mem), &pool));

	ptr[0] = ualloc(pool, 64);

	pool_dump(pool);

	ptr[1] = ualloc(pool, 64);

	pool_dump(pool);

	ptr[2] = ualloc(pool, 64);

	pool_dump(pool);

	ptr[3] = ualloc(pool, 64);

	pool_dump(pool);

	ptr[4] = ualloc(pool, 64);

	pool_dump(pool);

	ufree(pool, ptr[1]);

	pool_dump(pool);

	ufree(pool, ptr[2]);

	pool_dump(pool);

	ufree(pool, ptr[3]);

	pool_dump(pool);

	ufree(pool, ptr[0]);

	pool_dump(pool);

	ufree(pool, ptr[4]);

	pool_dump(pool);

	ptr[0] = ualloc(pool, 250);

	pool_dump(pool);

	ptr[1] = ualloc(pool, 3245);

	pool_dump(pool);

	ptr[2] = ualloc(pool, 64);

	pool_dump(pool);

	ufree(pool, ptr[1]);

	pool_dump(pool);

	ptr[1] = ualloc(pool, 3200);

	pool_dump(pool);

	ufree(pool, ptr[0]);

	pool_dump(pool);

	ufree(pool, ptr[1]);

	pool_dump(pool);

	ufree(pool, ptr[2]);

	pool_dump(pool);

	ptr[0] = ualloc(pool, 64);

	pool_dump(pool);

	ptr[1] = ualloc(pool, 64);

	pool_dump(pool);

	ptr[2] = ualloc(pool, 64);

	pool_dump(pool);

	ptr[3] = ualloc(pool, 64);

	pool_dump(pool);

	ptr[4] = ualloc(pool, 64);

	pool_dump(pool);

	ufree(pool, ptr[1]);

	pool_dump(pool);

	ufree(pool, ptr[2]);

	pool_dump(pool);

	ufree(pool, ptr[3]);

	pool_dump(pool);

	ufree(pool, ptr[0]);

	pool_dump(pool);

	ufree(pool, ptr[4]);

	pool_dump(pool);

	ptr[0] = ualloc(pool, 250);

	pool_dump(pool);

	ptr[1] = ualloc(pool, 3245);

	pool_dump(pool);

	ptr[2] = ualloc(pool, 64);

	pool_dump(pool);

	ufree(pool, ptr[1]);

	pool_dump(pool);

	ptr[1] = ualloc(pool, 3200);

	pool_dump(pool);

	ufree(pool, ptr[0]);

	pool_dump(pool);

	ufree(pool, ptr[1]);

	pool_dump(pool);

	ufree(pool, ptr[2]);

	pool_dump(pool);

	ptr[1] = ualloc(pool, 0xffff);

	pool_dump(pool);

	ufree(pool, ptr[1]);

	pool_dump(pool);

	ptr[1] = ualloc(pool, 10);

	pool_dump(pool);

	ptr[1] = urealloc(pool, ptr[1], 20);
	pool_dump(pool);

	ptr[1] = urealloc(pool, ptr[1], 30);
	pool_dump(pool);

	ptr[1] = urealloc(pool, ptr[1], 40);
	pool_dump(pool);

	ptr[2] = ualloc(pool, 64);
	pool_dump(pool);

	ptr[1] = urealloc(pool, ptr[1], 50);
	pool_dump(pool);

	urelease(pool);
	pool_dump(pool);

	return 0;
}