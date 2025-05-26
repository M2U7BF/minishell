/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:30:35 by kkamei            #+#    #+#             */
/*   Updated: 2024/10/25 14:56:25 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*rev_copy(char *dest, char *src, size_t n);

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char			*dest_mem;
	char			*src_mem;

	if (!dest && !src)
		return (NULL);
	dest_mem = (char *) dest;
	src_mem = (char *) src;
	if (dest_mem > src_mem && dest_mem < src_mem + n)
		return (rev_copy(dest_mem, src_mem, n));
	else
	{
		ft_memcpy(dest_mem, src_mem, n);
		return (dest_mem);
	}
}

static void	*rev_copy(char *dest, char *src, size_t n)
{
	size_t	i;

	i = n;
	while (i > 0)
	{
		dest[i - 1] = src[i - 1];
		i--;
	}
	return (dest);
}
