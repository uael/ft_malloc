/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   class.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:42 by alucas-           #+#    #+#             */
/*   Updated: 1970/01/01 00:00:42 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLASS_H
# define CLASS_H

# include "ualloc.h"

# define MAX_POOL  (1 << 8)
# define MIN_ALLOC (1 << 7)
# define ALIGN     (sizeof(t_chunk))

/*
** Allocation size classes definition
** Each class get it's own bin entries
** Tiny and small are divided in chunk of equal size, which are used or not
*/

enum	e_class
{
	CLASS_TINY  = 0x06,
	CLASS_SMALL = 0x09,
	CLASS_LARGE = 0x10,
};

enum	e_size
{
	SIZE_TINY  = (1 << CLASS_TINY ) * 32,
	SIZE_SMALL = (1 << CLASS_SMALL) * 32,
};

enum e_class	classof(size_t sz);

#endif
