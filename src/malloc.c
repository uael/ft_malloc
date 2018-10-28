/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:42 by alucas-           #+#    #+#             */
/*   Updated: 1970/01/01 00:00:42 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "ualloc.h"

void	free(void *ptr)
{
	ufree(ptr);
}

void	*malloc(size_t sz)
{
	return (ualloc(NULL, sz));
}

void	*realloc(void *ptr, size_t sz)
{
	return (urealloc(NULL, ptr, sz));
}

void	*calloc(size_t nb, size_t sz)
{
	return (ucalloc(NULL, nb, sz));
}

void	show_alloc_mem(void)
{

}
