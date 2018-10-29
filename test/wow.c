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
#include "../src/pool.h"

#include <assert.h>
#include <string.h>
#include <libft/io.h>
#include <malloc.h>

uint8_t g_mem[UINT32_MAX];

int main(int ac, char *av[])
{
	void *ptrs[64 * 64];
	unsigned i = 0;
	t_upool pool1, pool2;

	assert(0 == ustack(g_mem, sizeof(g_mem), &pool1));
	assert(0 == uheap(&pool2));

	assert(0 == upush(pool1));
	{
		ptrs[0] = malloc(32);
		ptrs[1] = malloc(64);
		ft_printf("POOL1:\n");
		show_alloc_mem();
		ft_printf("\n");

		assert(0 == upush(pool2));
		{
			ptrs[0] = malloc(32);
			ptrs[1] = malloc(64);

			ft_printf("POOL2:\n");
			show_alloc_mem();
			ft_printf("\n");
		}
		ft_printf("\n");
		assert(0 == upop());

		ft_printf("POOL1:\n");
		show_alloc_mem();
		ft_printf("\n");
	}
	assert(0 == upop());

	ft_printf("POOL1:\n");
	pool_dump(pool1);
	ft_printf("\n");

	ft_printf("POOL2:\n");
	pool_dump(pool2);
	ft_printf("\n");

	return 0;
}