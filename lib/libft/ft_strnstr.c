/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 10:38:39 by kkamei            #+#    #+#             */
/*   Updated: 2024/10/30 11:18:32 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t		i;
	size_t		lit_len;

	if (!*little)
		return ((char *)big);
	if (len == 0)
		return (NULL);
	i = 0;
	lit_len = ft_strlen(little);
	while (big[i] && i + lit_len <= len)
	{
		if (ft_strncmp(&big[i], little, lit_len) == 0)
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}
