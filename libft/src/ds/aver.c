/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aver.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 09:52:33 by alucas-           #+#    #+#             */
/*   Updated: 2017/11/07 09:53:34 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/ds.h"

inline size_t	ft_deqaver(t_deq *self, size_t n)
{
	if (++n < FT_DEQ_MIN_CAP && FT_DEQ_MIN_CAP > self->cap)
		return (ft_deqalloc(self, FT_DEQ_MIN_CAP));
	else if (n > self->cap)
		return (ft_deqalloc(self, ft_ispow2(n) ? n : pow2_next(n)));
	return (self->cap);
}

inline size_t	ft_sdsaver(t_sds *self, size_t n)
{
	if (++n < FT_SDS_MIN_CAP && FT_SDS_MIN_CAP > self->cap)
		return (ft_sdsalloc(self, FT_SDS_MIN_CAP));
	else if (n > self->cap)
		return (ft_sdsalloc(self, ft_ispow2(n) ? n : pow2_next(n)));
	return (self->cap);
}

inline size_t	ft_vecaver(t_vec *self, size_t n)
{
	if (++n < FT_VEC_MIN_CAP && FT_VEC_MIN_CAP > self->cap)
		return (ft_vecalloc(self, FT_VEC_MIN_CAP));
	else if (n > self->cap)
		return (ft_vecalloc(self, ft_ispow2(n) ? n : pow2_next(n)));
	return (self->cap);
}
