/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkamei <kkamei@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:51:18 by kkamei            #+#    #+#             */
/*   Updated: 2025/07/21 15:45:33 by kkamei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_chr(char *s, char c)
{
	int	i;
	int	count;

	if (!s)
		return (0);
	count = 0;
	i = -1;
	while (s[++i])
	{
		if (s[i] == c)
			count++;
	}
	return (count);
}

bool	is_include(char *s, char **words)
{
	int	i;
	int	str_len;

	str_len = ft_strlen(s);
	i = -1;
	while (words[++i])
	{
		if (ft_strncmp(s, words[i], str_len + 1) == 0)
			return (true);
	}
	return (false);
}

bool	is_s_eq(char *s1, char *s2, bool include_null_char)
{
	int	len;

	if (!s1 && !s2)
		return (false);
	if (!s1 || !s2)
		return (false);
	len = ft_strlen(s2);
	if (include_null_char)
		len++;
	return (ft_strncmp(s1, s2, len) == 0);
}

bool	is_quoted(char *s)
{
	return (ft_strchr(s, '"') != NULL || ft_strchr(s, '\'') != NULL);
}

void	trim_endl(char **s)
{
	char	*old;
	char	*new;

	if (!s || !s[0])
		return ;
	old = *s;
	if ((*s)[ft_strlen(*s) - 1] == '\n')
	{
		new = malloc(sizeof(char) * (ft_strlen(*s) + 1));
		ft_strlcpy(new, old, (ft_strlen(*s)));
		ft_free((void **)&old);
		*s = new;
	}
}
