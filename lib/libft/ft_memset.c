/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:31:25 by kkamei            #+#    #+#             */
/*   Updated: 2024/11/19 00:37:05 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	char		*dest;

	dest = (char *)s;
	while ((ssize_t)n > 0)
	{
		*dest = c;
		dest++;
		n--;
	}
	return (s);
}
