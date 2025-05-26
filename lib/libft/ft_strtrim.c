/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 08:44:16 by kkamei            #+#    #+#             */
/*   Updated: 2024/10/29 08:40:52 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_set(char c, char const *set);
static char	*get_start(const char *s1, const char *set);
static char	*get_end(const char *s1, const char *set);

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	char	*start;
	char	*end;
	int		result_len;

	start = get_start(s1, set);
	end = get_end(s1, set);
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

static int	is_set(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
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

static char	*get_end(const char *s1, const char *set)
{
	int		i;

	i = ft_strlen(s1) - 1;
	while (i >= 0)
	{
		if (!is_set(s1[i], set))
			return ((char *)&s1[i]);
		i--;
	}
	return (NULL);
}
