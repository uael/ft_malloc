/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ualloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:42 by alucas-           #+#    #+#             */
/*   Updated: 1970/01/01 00:00:42 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UALLOC_H
# define UALLOC_H

# include <stdlib.h>

struct			s_ualloc_mapper
{
	size_t		page_size;
	size_t		span_size;

	void		*(*mmap)(size_t size, size_t *off);
	void		*(*munmap)(void *addr, size_t size, size_t off, size_t release);
};

extern void		ualloc_init(const struct s_ualloc_mapper *mapper);
extern void		ualloc_destroy(void);

extern void		ufree(void *ptr);
extern void		*umalloc(size_t size) __attribute__((__malloc__));
extern void		*ucalloc(size_t num, size_t size) __attribute__((__malloc__));
extern void		*urealloc(void *ptr, size_t size) __attribute__((__malloc__));

#endif
