/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:32:15 by kkamei            #+#    #+#             */
/*   Updated: 2024/10/29 14:08:07 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_elem_count(char const *s, char c);
static char	*get_fragment(char *start, int *len, char *str);
static char	**free_all(char **arr, int i);

char	**ft_split(char const *s, char c)
{
	size_t	i;
	int		j;
	int		s_count;
	char	**str;

	i = 0;
	j = 0;
	s_count = 0;
	str = (char **)malloc(sizeof(char *) * (get_elem_count(s, c) + 1));
	if (!str)
		return (NULL);
	while (i <= ft_strlen(s))
	{
		if (s_count > 0 && (s[i] == c || !s[i]))
		{
			str[j] = get_fragment(((char *)&s[i] - s_count), &s_count, str[j]);
			if (!str[j++])
				return (free_all(str, j));
		}
		else if (s[i] != c)
			s_count++;
		i++;
	}
	str[j] = NULL;
	return (str);
}

static int	get_elem_count(char const *s, char c)
{
	size_t	i;
	int		elem_count;
	int		s_count;

	i = 0;
	elem_count = 0;
	s_count = 0;
	while (i < ft_strlen(s) + 1)
	{
		if (s_count > 0 && (s[i] == c || s[i] == '\0'))
		{
			elem_count++;
			s_count = 0;
		}
		else if (s[i] != c)
			s_count++;
		i++;
	}
	return (elem_count);
}

static char	*get_fragment(char *start, int *len, char *str)
{
	str = (char *)malloc(*len + 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, start, *len + 1);
	*len = 0;
	return (str);
}

static char	**free_all(char **arr, int i)
{
	while (i >= 0)
	{
		free(arr[i]);
		i--;
	}
	free(arr);
	return (NULL);
}
