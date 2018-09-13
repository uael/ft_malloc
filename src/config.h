/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:42 by alucas-           #+#    #+#             */
/*   Updated: 1970/01/01 00:00:42 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# include "ualloc.h"

# define UALLOC_SPAN_SIZE     ( 64 * 1024)
# define UALLOC_MAX_SPAN_SIZE (256 * 1024)

extern struct	s_ualloc
{
	size_t					span_size_shift;
	struct s_ualloc_mapper	mapper;

}				g_ualloc;

#endif
