/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:42 by alucas-           #+#    #+#             */
/*   Updated: 1970/01/01 00:00:42 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef chunk_H
# define chunk_H

# include <stddef.h>
# include <stdint.h>

typedef struct s_bin	t_bin;

typedef struct			s_chunk
{
	uint16_t			solo: 1;
	uint16_t			refc: 15;
	uint16_t			prv;
	uint16_t			off;
	uint16_t			nxt;
}						t_chunk;

extern t_bin			*chunk_bin(t_chunk *chunk);
extern uintptr_t		chunk_mem(t_chunk *chunk);
extern size_t			chunk_size(t_chunk *chunk);
extern t_chunk			*chunk_nxt(t_chunk *chunk, t_bin *bin);
extern t_chunk			*chunk_prv(t_chunk *chunk, t_bin *bin);

#endif
