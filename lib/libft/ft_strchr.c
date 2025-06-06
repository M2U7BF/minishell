/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 08:01:04 by kkamei            #+#    #+#             */
/*   Updated: 2024/10/29 22:49:04 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	char	*str;
	size_t	len;

  if (!s)
    return (NULL);
	i = 0;
	len = ft_strlen(s);
	str = (char *)s;
	while (i < len + 1)
	{
		if (str[i] == (char)c)
			return (str + i);
		i++;
	}
	return (NULL);
}
