/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multi_split_leave_separator.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 10:03:12 by kkamei            #+#    #+#             */
/*   Updated: 2025/06/27 11:25:50 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	is_separator(char *separator, char c)
{
	return (ft_strchr(separator, c) != NULL);
}

static int	get_elem_count(char const *s, char *separator)
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
		if (s_count > 0 && (is_separator(separator, s[i]) || s[i] == '\0'))
		{
			elem_count++;
			s_count = 0;
		}
		s_count++;
		i++;
	}
	return (elem_count);
}

static char	*get_fragment(char *str, char *start, int *len)
{
	str = (char *)malloc(*len + 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, start, *len + 1);
	*len = 0;
	return (str);
}

static char	**init_str_arr(int elem_count)
{
	char	**str;

	str = (char **)malloc(sizeof(char *) * (elem_count + 1));
	if (!str)
		return (NULL);
	str[elem_count] = NULL;
	return (str);
}

// char *s: 分割対象の文字列
// char *separator: 分割文字の配列
// 複数の文字にマッチして、文字列を分割する。
char	**ft_multi_split_leave_separator(char *s, char *separator)
{
	size_t	i;
	int		j;
	int		s_count;
	char	**str;

	i = -1;
	j = 0;
	s_count = 0;
	if (!s || !separator)
		return (NULL);
	str = init_str_arr(get_elem_count(s, separator));
	if (!str)
		return (NULL);
	while (get_elem_count(s, separator) > 0 && ++i <= ft_strlen(s))
	{
		if (s_count > 0 && (is_separator(separator, s[i]) || !s[i]))
		{
			str[j] = get_fragment(str[j], s + i - s_count, &s_count);
			if (!str[j++])
				return (free_str_array(str), NULL);
		}
		s_count++;
	}
	return (str);
}
