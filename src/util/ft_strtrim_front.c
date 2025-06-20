/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim_front.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 09:36:39 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/19 13:32:34 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	is_set(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (true);
		i++;
	}
	return (false);
}

static char	*get_start(const char *s1, const char *set)
{
	int		i;

	i = 0;
	while (s1[i])
	{
		if (!is_set(s1[i], set))
			return ((char *)&s1[i]);
		i++;
	}
	return (NULL);
}

char	*ft_strtrim_front(char const *s1, char const *set)
{
	char	*str;
	char	*start;
	char	*end;
	int		result_len;

	start = get_start(s1, set);
  end = (char *)s1 + ft_strlen(s1);
	result_len = end - start + 1;
	if (!start || !end)
		result_len = 0;
	str = (char *)malloc(sizeof(char) * (result_len + 1));
	if (!str)
		return (NULL);
	ft_memcpy(str, start, result_len);
	str[result_len] = '\0';
	return (str);
}
