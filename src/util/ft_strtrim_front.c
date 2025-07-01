/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim_front.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 09:36:39 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/01 10:10:37 by kkamei           ###   ########.fr       */
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
	int	i;

	i = 0;
	while (s1[i])
	{
		if (!is_set(s1[i], set))
			return ((char *)&s1[i]);
		i++;
	}
	return (NULL);
}

void	ft_strtrim_front(char **s1, char *set)
{
	char	*str;
	char	*start;
	char	*end;
	int		result_len;

	start = get_start(*s1, set);
	end = *s1 + ft_strlen(*s1);
	result_len = end - start + 1;
	if (!start || !end)
		result_len = 0;
	str = malloc(sizeof(char) * (result_len + 1));
	if (str)
  {
    ft_memcpy(str, start, result_len);
    str[result_len] = '\0';
  }
  ft_free((void **)s1);
  *s1 = str;
}
