/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:30:35 by kkamei            #+#    #+#             */
/*   Updated: 2024/10/29 21:51:52 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char			*mem1;
	char			*mem2;
	unsigned int	i;

	if (!dest && !src)
		return (NULL);
	mem1 = (char *)dest;
	mem2 = (char *)src;
	i = 0;
	while (i < (ssize_t)n)
	{
		mem1[i] = mem2[i];
		i++;
	}
	return (mem1);
}
