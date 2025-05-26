/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 07:22:43 by kkamei            #+#    #+#             */
/*   Updated: 2024/10/29 08:21:13 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*mem;
	size_t	bytes;

	bytes = nmemb * size;
	if (nmemb == 0 || size == 0)
		bytes = 1;
	else if (bytes / nmemb != size)
		return (NULL);
	mem = malloc(bytes);
	if (!mem)
		return (NULL);
	return (ft_memset(mem, 0, bytes));
}
