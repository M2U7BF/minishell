/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_leave_separator.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 10:03:12 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/26 09:04:44 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	get_elem_count(char const *s, char c)
{
	size_t	i;
	int		elem_count;
	int		s_count;

	i = 0;
	elem_count = 0;
	s_count = 0;
	if (!s)
		return (0);
	while (i < ft_strlen(s) + 1)
	{
		if (s_count > 0 && (s[i] == c || s[i] == '\0'))
		{
			elem_count++;
			s_count = 0;
		}
		s_count++;
		i++;
	}
	return (elem_count);
}

static char	*get_fragment(char *str, char *start, int len)
{
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, start, len + 1);
	return (str);
}

static char	**free_all(char **arr, int i)
{
	while (i >= 0)
	{
		ft_free(arr[i]);
		i--;
	}
	ft_free(arr);
	return (NULL);
}

char	**ft_split_leave_separator(char *s, char separator)
{
	size_t	i;
	int		j;
	int		s_count;
	char	**str;

	i = 0;
	j = 0;
	s_count = 0;
	str = (char **)malloc(sizeof(char *) * (get_elem_count(s, separator) + 1));
	if (!str)
		return (NULL);
	while (get_elem_count(s, separator) > 0 && i <= ft_strlen(s))
	{
		if (s_count > 0 && (s[i] == separator || !s[i]))
		{
			str[j] = get_fragment(str[j], s + i - s_count, s_count);
			s_count = 0;
			if (!str[j++])
				return (free_all(str, j));
		}
		s_count++;
		i++;
	}
	str[j] = NULL;
	return (str);
}
